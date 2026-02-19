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
    gain.setCurrentAndTargetValue(0.5f);
    quality.setCurrentAndTargetValue(0.5f);
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
    auto coeffs = juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, frequency.getCurrentValue());

    *filter.state = *coeffs;
}

void EQBand::process(juce::dsp::ProcessContextReplacing<float>& context) {
    filter.process(context);
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
