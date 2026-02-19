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

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;

    std::vector<EQBand> bands;
    double sampleRate = 44100.0;
};