/*
  ==============================================================================

    EQBand.h
    Created: 18 Feb 2026 4:03:26pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "FilterType.h"


class EQBand {
public:
    EQBand();

    void prepare(double sr, int samplesPerBlock, int numChannels);
    void updateCoefficients();
    void process(juce::dsp::ProcessContextReplacing<float>& context);

    void setFilterType(FilterType newType);
    /// <summary>
    /// Returns the biquad coefficients of the filter in order of b0, b1, b2, a1, a2
    /// </summary>
    /// <param name="coeffVec">The vector to place the coefficients inside</param>
    void getFilterCoefficients(std::vector<float>& coeffVec) const;

    /// <summary>
    /// Sets the central/cutoff frequency of the band in Hz
    /// </summary>
    /// <param name="newFreq">The new frequency to assign to the EQ Band</param>
    void setFrequency(float newFreq);
    /// <summary>
    /// Sets the gain of the EQ band in dB
    /// </summary>
    /// <param name="newGain">The new gain to assignt to the EQ band</param>
    void setGain(float newGain);
    /// <summary>
    /// Sets the Q factor of the EQ band.
    /// </summary>
    /// <param name="newQuality">The new Q factor to assign to the EQ band</param>
    void setQuality(float newQuality);

private:
    juce::dsp::ProcessorDuplicator<
        juce::dsp::IIR::Filter<float>,
        juce::dsp::IIR::Coefficients<float>> filter;
    
    double sampleRate = 44100.0;
    int preparedChannels = 0;

    FilterType type = FilterType::Peak;  // The type of filter this band uses
    bool updateFilter = true;  // Should the filter be updated in the next updateCoefficients call?

    juce::SmoothedValue<float> frequency;
    juce::SmoothedValue<float> gain;
    juce::SmoothedValue<float> quality;  // Quality factor (Q)
};