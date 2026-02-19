/*
  ==============================================================================

    EQBand.h
    Created: 18 Feb 2026 4:03:26pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


enum class FilterType {
    HighPass,
    HighShelf,
    Peak,
    LowShelf,
    LowPass
};


class EQBand {
public:
    EQBand();

    void prepare(double sr, int samplesPerBlock, int numChannels);
    void updateCoefficients();
    void process(juce::dsp::ProcessContextReplacing<float>& context);

    void setFilterType(FilterType newType);

    void setFrequency(float newFreq);
    void setGain(float newGain);
    void setQuality(float newQuality);

private:
    juce::dsp::ProcessorDuplicator<
        juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Coefficients<float>> filter;
    
    double sampleRate = 44100.0;
    int preparedChannels = 0;

    FilterType type = FilterType::LowPass;  // The type of filter this band uses
    bool updateFilter = true;  // Should the filter be updated in the next updateCoefficients call?

    juce::SmoothedValue<float> frequency;
    juce::SmoothedValue<float> gain;
    juce::SmoothedValue<float> quality;  // Quality factor (Q)
};