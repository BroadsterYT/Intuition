/*
  ==============================================================================

    WavetableHelper.cpp
    Created: 15 Oct 2025 1:10:30am
    Author:  BroDe

  ==============================================================================
*/

#include "WavetableHelper.h"


void WavetableHelper::preprocessWavetable(juce::AudioBuffer<float>& wavetable) {
    int numSamples = wavetable.getNumSamples();
    int numChannels = wavetable.getNumChannels();

    for (int ch = 0; ch < numChannels; ++ch) {
        float* samples = wavetable.getWritePointer(ch);

        // Remove DC offset
        float mean = 0.0f;
        for (int i = 0; i < numSamples; ++i) {
            mean += samples[i];
        }
        mean /= numSamples;

        for (int i = 0; i < numSamples; ++i) {
            samples[i] -= mean;
        }

        // Normalize peak amplitude to [-1, 1]
        float maxAmp = 0.0f;
        for (int i = 0; i < numSamples; ++i) {
            maxAmp = std::max(maxAmp, std::abs(samples[i]));
        }

        if (maxAmp > 0.0f) {
            for (int i = 0; i < numSamples; ++i) {
                samples[i] /= maxAmp;
            }
        }
    }
}

void WavetableHelper::phaseAlignWavetable(juce::AudioBuffer<float>& wavetable) {
    int numSamples = wavetable.getNumSamples();
    int numChannels = wavetable.getNumChannels();

    for (int ch = 0; ch < numChannels; ++ch) {
        float* samples = wavetable.getWritePointer(ch);

        // Find first positive-going zero-crossing
        int zeroIndex = 0;
        for (int i = 0; i < numSamples - 1; ++i) {
            if (samples[i] <= 0.0f && samples[i + 1] > 0.0f) {
                zeroIndex = i;
                break;
            }
        }

        // Rotate the buffer so it starts at that zero-crossing
        juce::HeapBlock<float> temp(numSamples);
        for (int i = 0; i < numSamples; ++i) {
            temp[i] = samples[(i + zeroIndex) % numSamples];
        }

        //memcpy(samples, temp, sizeof(float) * numSamples);
        std::copy(temp.get(), temp.get() + numSamples, samples);
    }
}