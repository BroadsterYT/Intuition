/*
  ==============================================================================

    EQBand.cpp
    Created: 18 Feb 2026 4:03:26pm
    Author:  BroDe

  ==============================================================================
*/

#include "EQBand.h"


EQBand::EQBand() {
    frequency.setCurrentAndTargetValue(1000.0f);
    gain.setCurrentAndTargetValue(0.0f);
    quality.setCurrentAndTargetValue(0.707f);
}

void EQBand::prepare(double sr, int samplesPerBlock, int numChannels) {
    if (preparedChannels != numChannels) {
        sampleRate = sr;
        frequency.reset(sr, 0.02);  // 20ms smoothing
        gain.reset(sr, 0.02);
        quality.reset(sr, 0.02);

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

    float curFreq = frequency.getCurrentValue();
    float curGain = juce::Decibels::decibelsToGain(gain.getCurrentValue());
    float curQ = quality.getCurrentValue();

    switch (type) {
    case FilterType::HighPass:
        coeffs = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, curFreq, curQ);
        break;
    case FilterType::HighShelf:
        coeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, curFreq, curQ, curGain);
        break;
    case FilterType::Peak:
        coeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, curFreq, curQ, curGain);
        break;
    case FilterType::LowShelf:
        coeffs = juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, curFreq, curQ, curGain);
        break;
    case FilterType::LowPass:
        coeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, curFreq, curQ);
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

void EQBand::getFilterCoefficients(std::vector<float>& coeffVec) const {
    auto coeffs = filter.state;
    coeffVec.push_back(coeffs->coefficients[0]);  // b0
    coeffVec.push_back(coeffs->coefficients[1]);  // b1
    coeffVec.push_back(coeffs->coefficients[2]);  // b2
    coeffVec.push_back(coeffs->coefficients[3]);  // a1
    coeffVec.push_back(coeffs->coefficients[4]);  // a2
}

void EQBand::setFrequency(float newFreq) {
    //if (newFreq < 0) newFreq = 1.0f;
    frequency.setTargetValue(newFreq);
}

void EQBand::setGain(float newGain) {
    gain.setTargetValue(newGain);
}

void EQBand::setQuality(float newQuality) {
    quality.setTargetValue(newQuality);
}
