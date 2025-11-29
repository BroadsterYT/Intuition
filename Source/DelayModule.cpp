/*
  ==============================================================================

    DelayModule.cpp
    Created: 14 Nov 2025 6:14:04pm
    Author:  BroDe

  ==============================================================================
*/

#include "DelayModule.h"


DelayModule::DelayModule(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix) : parameters(vts), modMatrix(modMatrix) {

}

void DelayModule::prepare(double sr, float maxDelayMs, int numChan) {
    sampleRate = sr;
    numChannels = numChan;

    maxDelaySamples = (int)((maxDelayMs / 1000.0f) * sampleRate);
    delayBuffer.setSize(numChannels, maxDelaySamples);
    delayBuffer.clear();

    writePos = 0;

    for (int ch = 0; ch < numChannels; ++ch) {
        feedbackFilter[ch].reset();
    }
}

void DelayModule::updateParameters() {
    float delayTimeMs = modMatrix->getModdedDest("DELAY_TIME_MS");
    float fb = modMatrix->getModdedDest("DELAY_FEEDBACK");
    float cutoffHz = modMatrix->getModdedDest("DELAY_CUTOFF");
    float dryLevel = modMatrix->getModdedDest("DELAY_DRY_LEVEL");
    float wetLevel = modMatrix->getModdedDest("DELAY_WET_LEVEL");

    delayTimeSamples = (delayTimeMs / 1000.0f) * sampleRate;
    feedback = juce::jlimit(0.0f, 0.99f, fb);
    dry = dryLevel;
    wet = wetLevel;

    for (int ch = 0; ch < numChannels; ++ch) {
        feedbackFilter[ch].setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, cutoffHz));
    }
}

void DelayModule::processBlock(juce::AudioBuffer<float>& buffer) {
    int numSamples = buffer.getNumSamples();

    for (int n = 0; n < numSamples; ++n) {
        float readPos = writePos - delayTimeSamples;
        if (readPos < 0) readPos += maxDelaySamples;

        int idx1 = (int)readPos;
        int idx2 = (idx1 + 1) % maxDelaySamples;
        float frac = readPos - idx1;

        for (int ch = 0; ch < numChannels; ++ch) {
            float delayedSample = delayBuffer.getSample(ch, idx1) * (1.0f - frac) 
                                + delayBuffer.getSample(ch, idx2) * frac;

            float input = buffer.getSample(ch, n);
            float fb = feedbackFilter[ch].processSingleSampleRaw(delayedSample * feedback);

            delayBuffer.setSample(ch, writePos, input + fb);

            float dryOut = input * dry;
            float wetOut = delayedSample * wet;
            buffer.setSample(ch, n, dryOut + wetOut);
        }

        writePos = (writePos + 1) % maxDelaySamples;
    }
}
