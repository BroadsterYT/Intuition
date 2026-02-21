/*
  ==============================================================================

    EqualizerModule.cpp
    Created: 18 Feb 2026 4:14:56pm
    Author:  BroDe

  ==============================================================================
*/

#include "EqualizerModule.h"


EqualizerModule::EqualizerModule(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix) : parameters(vts), modMatrix(modMatrix) {
    bands.push_back(EQBand());
    bands[0].setFilterType(FilterType::LowPass);
    bands[0].setFrequency(500.0f);

    bands.push_back(EQBand());
    bands[1].setFilterType(FilterType::HighPass);
    bands[1].setFrequency(300.0f);
    bands[1].setQuality(0.707f);
}

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

void EqualizerModule::getBandCoefficients(std::vector<std::vector<float>>& bandCoeffs) {
    for (auto& band : bands) {
        std::vector<float> coeffs;
        band.getFilterCoefficients(coeffs);
        jassert(coeffs.size() > 0);
        bandCoeffs.push_back(coeffs);
    }
}