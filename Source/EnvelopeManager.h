/*
  ==============================================================================

    EnvelopeManager.h
    Created: 24 Oct 2025 2:01:21pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include "GlobalEnvelope.h"


class EnvelopeManager {
public:
    EnvelopeManager(juce::AudioProcessorValueTreeState& vts);

    void prepare(double newSampleRate);
    void setParameters();

    double getSampleRate() const;

    void noteOn(int noteNumber);
    void noteOff(int noteNumber);

    void process(int numSamples);
    float getEnvValue(size_t index) const;
    const GlobalEnvelope& getEnv(size_t index);

private:
    juce::AudioProcessorValueTreeState& parameters;
    std::array<GlobalEnvelope, 4> envs;
    double sampleRate = 44100.0;

    std::array<float, 4> lastValues = { 0.0f, 0.0f, 0.0f, 0.0f };

    std::unordered_set<int> activeNotes;
};
