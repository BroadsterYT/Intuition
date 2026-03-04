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
    void updateParameters();
    void processBlock(juce::AudioBuffer<float>& buffer);

    const EQBand& getBand(int bandIndex);

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;

    std::vector<EQBand> bands;
    double sampleRate = 44100.0;

    /// <summary>
    /// Sets the filter type of a band to the type specified by a filter type parameter. This handles the conversion
    /// between the AudioParameterChoice and FilterType enum.
    /// </summary>
    /// <param name="bandIndex">The index of the band to set the filter type for</param>
    /// <param name="filterParamName">The parameter name of the AudioParameterChoice to select the filter type from</param>
    void setBandFilterType(int bandIndex, const juce::String& filterParamName);
};