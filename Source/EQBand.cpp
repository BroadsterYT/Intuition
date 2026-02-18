/*
  ==============================================================================

    EQBand.cpp
    Created: 18 Feb 2026 4:03:26pm
    Author:  BroDe

  ==============================================================================
*/

#include "EQBand.h"


void EQBand::prepare(double sr, int samplesPerBlock, int numChannels) {
    sampleRate = sr;
    frequency.reset(sr, 0.02);  // 20ms smoothing
    gain.reset(sr, 0.02);
    quality.reset(sr, 0.02);

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sr;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = numChannels;
    filter.prepare(spec);
}

void EQBand::updateCoefficients() {
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        sampleRate,
        frequency.getCurrentValue(),
        quality.getCurrentValue(),
        juce::Decibels::decibelsToGain(gain.getCurrentValue())
    );

    *filter.coefficients = *coeffs;
}

void EQBand::process(juce::dsp::ProcessContextReplacing<float>& context) {
    filter.process(context);
}
