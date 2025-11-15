/*
  ==============================================================================

    DelayModule.h
    Created: 14 Nov 2025 6:14:04pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ModMatrix.h"


class DelayModule {
public:
    DelayModule(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix);

    void prepare(double sr, float maxDelayMs, int numChan);
    void updateParameters();
    void processBlock(juce::AudioBuffer<float>& buffer);

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;

    juce::AudioBuffer<float> delayBuffer;
    juce::IIRFilter feedbackFilter[2];

    int numChannels = 2;
    double sampleRate = 44100.0;
    int writePos = 0;
    int maxDelaySamples = 0;
    int delayTimeSamples = 0;

    float feedback = 0.5f;
    float wet = 0.3f;
};