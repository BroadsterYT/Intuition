/*
  ==============================================================================

    EqualizerModule.h
    Created: 18 Feb 2026 4:14:56pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "EQBand.h"
#include "ModMatrix.h"


class EqualizerModule {
public:
    EqualizerModule(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix);

    void prepare(double sr, int samplesPerBlock, int numChannels);
    void processBlock(juce::AudioBuffer<float>& buffer);

    /// <summary>
    /// Returns the biquad coefficients of each EQ band
    /// </summary>
    /// <param name="bandCoeffs">The nested vector to place the coefficients. Will be in same order as bands in eq module.</param>
    void getBandCoefficients(std::vector<std::vector<float>>& bandCoeffs);

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;

    std::vector<EQBand> bands;
    double sampleRate = 44100.0;
};