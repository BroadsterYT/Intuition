/*
  ==============================================================================

    FilterGraph.h
    Created: 19 Oct 2025 3:37:10pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class FilterGraph : public juce::Component, private juce::Timer {
public:
    FilterGraph(juce::AudioProcessorValueTreeState& vts);

    void paint(juce::Graphics& g) override;

private:
    juce::AudioProcessorValueTreeState& parameters;

    void timerCallback() override;

    float magnitudeToDecibels(float mag);
    float getLowpassMagnitude(float freq, float cutoff, float resonance);
    float getHighpassMagnitude(float freq, float cutoff, float resonance);
    float getBandpassMagnitude(float freq, float cutoff, float resonance);
    void drawFrequencyResponse(juce::Graphics& g, float cutoff, float resonance, int filterType);
};