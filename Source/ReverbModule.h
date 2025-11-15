/*
  ==============================================================================

    ReverbModule.h
    Created: 30 Oct 2025 5:35:58pm
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ModMatrix.h"


class ReverbModule {
public:
    ReverbModule(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix);

    void prepare(double sampleRate, int samplesPerBlock, int numChannels);
    void updateParameters();
    void processBlock(juce::AudioBuffer<float>& buffer);

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;

    juce::dsp::Reverb reverb;
    juce::dsp::Reverb::Parameters reverbParams;

    int preparedChannels = 0;
};