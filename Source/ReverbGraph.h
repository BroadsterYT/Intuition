/*
  ==============================================================================

    ReverbGraph.h
    Created: 15 Nov 2025 3:51:54pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnLookAndFeel.h"
#include "ModMatrix.h"
#include "ReverbRing.h"


class ReverbGraph : public juce::Component, private juce::Timer {
public:
    ReverbGraph(ModMatrix* modMatrix);
    ~ReverbGraph();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    ItnLookAndFeel lookAndFeel;
    ModMatrix* modMatrix = nullptr;

    juce::OwnedArray<ReverbRing> rings;
    float spawnIntervalMs = 500.0f;
    juce::uint32 lastSpawnTime = 0;

    void timerCallback() override;
};