/*
  ==============================================================================

    ChrousModule.cpp
    Created: 30 Jan 2026 6:31:45pm
    Author:  BroDe

  ==============================================================================
*/

#include "ChorusModule.h"

ChorusModule::ChorusModule(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix) : parameters(vts), modMatrix(modMatrix) {

}

void ChorusModule::reset() {
    delayL.reset();
    delayR.reset();
    lfoPhase = 0.0f;
}

void ChorusModule::prepare(double sr, int samplesPerBlock) {
    if (sr != sampleRate) {
        sampleRate = sr;
        const int maxDelaySamples = static_cast<int>(sampleRate * maxDelayMs * 0.001f) + 1;

        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = samplesPerBlock;
        spec.numChannels = 1;  // Keep L and R mono

        delayL.prepare(spec);
        delayR.prepare(spec);

        delayL.setMaximumDelayInSamples(maxDelaySamples);
        delayR.setMaximumDelayInSamples(maxDelaySamples);

        reset();
    }
}

void ChorusModule::updateParameters() {
    rate = modMatrix->getModdedDest("CHORUS_RATE");
    depthMs = modMatrix->getModdedDest("CHORUS_DEPTH");
    width = modMatrix->getModdedDest("CHORUS_WIDTH");
    dryLevel = modMatrix->getModdedDest("CHORUS_DRY_LEVEL");
    wetLevel = modMatrix->getModdedDest("CHORUS_WET_LEVEL");

    lfoIncrement = juce::MathConstants<float>::twoPi * rate / (float)sampleRate;
}

void ChorusModule::processBlock(juce::AudioBuffer<float>& buffer) {
    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    auto* left = buffer.getWritePointer(0);
    auto* right = numChannels > 1 ? buffer.getWritePointer(1) : nullptr;

    for (int i = 0; i < numSamples; ++i) {
        float lfo = std::sin(lfoPhase);
        lfoPhase += lfoIncrement;
        if (lfoPhase > juce::MathConstants<float>::twoPi) {
            lfoPhase -= juce::MathConstants<float>::twoPi;
        }

        // Phase offset for stereo width
        float lfoR = std::sin(lfoPhase + width * juce::MathConstants<float>::halfPi);

        float delayLms = baseDelayMs + depthMs * lfo;
        float delayRms = baseDelayMs + depthMs * lfoR;
        delayL.setDelay(delayLms * (float)sampleRate * 0.001f);
        delayR.setDelay(delayRms * (float)sampleRate * 0.001f);

        //=== Process left
        float dryL = left[i];
        delayL.pushSample(0, dryL);
        float wetL = delayL.popSample(0);
        left[i] = dryL * dryLevel + wetL * wetLevel;

        //=== Process right
        if (right) {
            float dryR = right[i];
            delayR.pushSample(0, dryR);
            float wetR = delayR.popSample(0);
            right[i] = dryR * dryLevel + wetR * wetLevel;
        }
    }
}
