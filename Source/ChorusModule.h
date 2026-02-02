/*
  ==============================================================================

    ChrousModule.h
    Created: 30 Jan 2026 6:31:45pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ModMatrix.h"


class ChorusModule {
public:
    ChorusModule(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix);

    void reset();
    void prepare(double sr, int samplesPerBlock);
    void updateParameters();
    void processBlock(juce::AudioBuffer<float>& buffer);

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;

    double sampleRate = 44100.0;
    static constexpr float maxDelayMs = 40.0f;

    // User editable parameters
    float rate = 0.3f;
    float depthMs = 3.0f;
    float width = 7.0f;
    float dryLevel = 0.7f;
    float wetLevel = 0.3f;

    juce::dsp::DelayLine<float> delayL;
    juce::dsp::DelayLine<float> delayR;

    float lfoPhase = 0.0f;
    float lfoIncrement = 0.0f;

    float baseDelayMs = 15.0f;
};
