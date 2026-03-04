/*
  ==============================================================================

    EQBand.cpp
    Created: 18 Feb 2026 4:03:26pm
    Author:  BroDe

  ==============================================================================
*/

#include "EQBand.h"


EQBand::EQBand() {
}

void EQBand::prepare(double sr, int samplesPerBlock, int numChannels) {
    if (preparedChannels != numChannels) {
        sampleRate = sr;

        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sr;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = numChannels;
        filter.prepare(spec);

        preparedChannels = numChannels;
    }
}

void EQBand::updateCoefficients() {
    juce::ReferenceCountedObjectPtr<juce::dsp::IIR::Coefficients<float>> coeffs;

    float gainAmp = juce::Decibels::decibelsToGain(gain);

    switch (type) {
    case FilterType::HighPass:
        coeffs = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, frequency, quality);
        break;
    case FilterType::HighShelf:
        coeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, frequency, quality, gainAmp);
        break;
    case FilterType::Peaking:
        coeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, frequency, quality, gainAmp);
        break;
    case FilterType::LowShelf:
        coeffs = juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, frequency, quality, gainAmp);
        break;
    case FilterType::LowPass:
        coeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, frequency, quality);
        break;
    }

    *filter.state = *coeffs;
    if (updateFilter) {
        filter.reset();
        updateFilter = false;
    }
}

void EQBand::process(juce::dsp::ProcessContextReplacing<float>& context) {
    filter.process(context);
}

void EQBand::setFilterType(FilterType newType) {
    type = newType;
    updateFilter = true;
}

void EQBand::getBiquadCoefficients(std::vector<float>& coeffVec) const {
    auto coeffs = filter.state;
    coeffVec.push_back(coeffs->coefficients[0]);  // b0
    coeffVec.push_back(coeffs->coefficients[1]);  // b1
    coeffVec.push_back(coeffs->coefficients[2]);  // b2
    coeffVec.push_back(coeffs->coefficients[3]);  // a1
    coeffVec.push_back(coeffs->coefficients[4]);  // a2
}

void EQBand::setFrequency(float newFreq) {
    frequency = newFreq;
}

void EQBand::setGain(float newGain) {
    gain = newGain;
}

void EQBand::setQuality(float newQuality) {
    quality = newQuality;
}

float EQBand::getFrequency() const {
    return frequency;
}

float EQBand::getGain() const {
    return gain;
}

float EQBand::getQuality() const {
    return quality;
}