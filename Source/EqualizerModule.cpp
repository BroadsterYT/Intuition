/*
  ==============================================================================

    EqualizerModule.cpp
    Created: 18 Feb 2026 4:14:56pm
    Author:  BroDe

  ==============================================================================
*/

#include "EqualizerModule.h"


void EqualizerModule::prepare(double sr, int samplesPerBlock, int numChannels) {
    sampleRate = sr;

    for (auto& band : bands) {
        band.prepare(sr, samplesPerBlock, numChannels);
    }
}

void EqualizerModule::processBlock(juce::AudioBuffer<float>& buffer) {
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    for (auto& band : bands) {
        band.updateCoefficients();
        band.process(context);
    }
}