/*
  ==============================================================================

    DelayGraph.h
    Created: 15 Nov 2025 3:48:23pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ModMatrix.h"
#include "ItnLookAndFeel.h"
#include "DelayRing.h"


class DelayGraph : public juce::Component, private juce::Timer {
public:
    DelayGraph(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix);
    ~DelayGraph();

    void paint(juce::Graphics& g) override;

private:
    ItnLookAndFeel lookAndFeel;
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;

    juce::OwnedArray<DelayRing> rings;
    float spawnIntervalMs = 500.0f;
    juce::uint32 lastSpawnTime = 0;

    void timerCallback() override;
};