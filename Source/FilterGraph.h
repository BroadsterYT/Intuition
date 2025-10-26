/*
  ==============================================================================

    FilterGraph.h
    Created: 19 Oct 2025 3:37:10pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnLookAndFeel.h"


class FilterGraph : public juce::Component, private juce::Timer {
public:
    FilterGraph(juce::AudioProcessorValueTreeState& vts);
    ~FilterGraph();

    void paint(juce::Graphics& g) override;

private:
    ItnLookAndFeel lookAndFeel;
    juce::AudioProcessorValueTreeState& parameters;

    void timerCallback() override;
};