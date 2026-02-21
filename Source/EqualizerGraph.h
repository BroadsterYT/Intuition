/*
  ==============================================================================

    EqualizerGraph.h
    Created: 21 Feb 2026 1:23:40pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnLookAndFeel.h"
#include "ModMatrix.h"
#include "EqualizerModule.h"


class EqualizerGraph : public juce::Component, private juce::Timer {
public:
    EqualizerGraph(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix, EqualizerModule& equalizer);
    ~EqualizerGraph();

    void paint(juce::Graphics& g) override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;

    EqualizerModule& equalizer;

    void timerCallback() override;
};
