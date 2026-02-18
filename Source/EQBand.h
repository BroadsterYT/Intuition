/*
  ==============================================================================

    EQBand.h
    Created: 18 Feb 2026 4:03:26pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class EQBand {
public:
    void prepare(double sr, int samplesPerBlock, int numChannels);
    void updateCoefficients();
    void process(juce::dsp::ProcessContextReplacing<float>& context);

private:
    juce::dsp::IIR::Filter<float> filter;
    double sampleRate = 44100.0;

    juce::SmoothedValue<float> frequency;
    juce::SmoothedValue<float> gain;
    juce::SmoothedValue<float> quality;  // Quality factor (Q)
};