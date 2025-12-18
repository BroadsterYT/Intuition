/*
  ==============================================================================

    DelayGraph.cpp
    Created: 15 Nov 2025 3:48:23pm
    Author:  BroDe

  ==============================================================================
*/

#include "DelayGraph.h"

DelayGraph::DelayGraph(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix) : parameters(vts), modMatrix(modMatrix) {
    setLookAndFeel(&ItnLookAndFeel::getInstance());
    startTimerHz(60);
}

DelayGraph::~DelayGraph() {
    setLookAndFeel(nullptr);
}

void DelayGraph::paint(juce::Graphics& g) {
    g.fillAll(MinimalStyle::roomDark);
}

void DelayGraph::timerCallback() {
    spawnIntervalMs = modMatrix->getModdedDest("DELAY_TIME_MS");

    auto now = juce::Time::getMillisecondCounter();
    if (now - lastSpawnTime >= (juce::uint32)spawnIntervalMs) {
        auto newRing = new DelayRing(modMatrix);
        addAndMakeVisible(newRing);
        newRing->setBounds(getLocalBounds().reduced(10));
        newRing->toBack();
        rings.add(newRing);

        lastSpawnTime = now;

        // Removing rings that are invisible
        std::vector<DelayRing*> toDelete;
        for (auto& ring : rings) {
            if (ring->getBrightness() <= 0.01f) {
                toDelete.push_back(ring);
                //DBG("Ring deleted");
            }
        }
        for (auto& ring : toDelete) {
            rings.removeObject(ring, true);
        }
    }

    // Dimming rings over time
    float damp = modMatrix->getModdedDest("DELAY_FEEDBACK");
    float dampMap = juce::jmap(damp, 0.98f, 0.86f);
    for (auto& ring : rings) {
        float b = ring->getBrightness();
        ring->setBrightness(b * dampMap);
    }
}
