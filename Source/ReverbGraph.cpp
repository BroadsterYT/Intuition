/*
  ==============================================================================

    ReverbGraph.cpp
    Created: 15 Nov 2025 3:51:54pm
    Author:  BroDe

  ==============================================================================
*/

#include "ReverbGraph.h"

ReverbGraph::ReverbGraph(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix) : parameters(vts), modMatrix(modMatrix) {
    setLookAndFeel(&ItnLookAndFeel::getInstance());
    startTimerHz(60);
    lastSpawnTime = juce::Time::getMillisecondCounter();
}

ReverbGraph::~ReverbGraph() {
    setLookAndFeel(nullptr);
}

void ReverbGraph::paint(juce::Graphics& g) {
    g.fillAll(MinimalStyle::roomDark);
}

void ReverbGraph::timerCallback() {
    float wet = modMatrix->getModdedDest("REVERB_WET_LEVEL");
    spawnIntervalMs = juce::jmap(wet, 0.0f, 1.0f, 1000.0f, 200.0f);

    auto now = juce::Time::getMillisecondCounter();
    if (now - lastSpawnTime >= (juce::uint32)spawnIntervalMs) {
        auto newRing = new ReverbRing(modMatrix);
        addAndMakeVisible(newRing);
        newRing->setBounds(getLocalBounds().reduced(10));
        newRing->toBack();
        rings.add(newRing);
        
        lastSpawnTime = now;

        // Removing rings that are too big
        std::vector<ReverbRing*> toDelete;
        for (auto& ring : rings) {
            if (ring->getRadius() * 2.0 >= getWidth()) {
                toDelete.push_back(ring);
            }
        }
        for (auto& ring : toDelete) {
            rings.removeObject(ring, true);
        }
    }

    // Dimming rings over time
    float damp = modMatrix->getModdedDest("REVERB_DAMPING");
    float dampMap = juce::jmap(damp, 0.98f, 0.86f);
    for (auto& ring : rings) {
        float b = ring->getBrightness();
        ring->setBrightness(b * dampMap);
    }
}
