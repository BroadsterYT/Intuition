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
    EQBand();

    void prepare(double sr, int samplesPerBlock, int numChannels);
    void updateCoefficients();
    void process(juce::dsp::ProcessContextReplacing<float>& context);

    void setFrequency(float newFreq);
    void setGain(float newGain);
    void setQuality(float newQuality);

private:
    juce::dsp::ProcessorDuplicator<
        juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Coefficients<float>> filter;
    double sampleRate = 44100.0;

    int preparedChannels = 0;

    juce::SmoothedValue<float> frequency;
    juce::SmoothedValue<float> gain;
    juce::SmoothedValue<float> quality;  // Quality factor (Q)
};