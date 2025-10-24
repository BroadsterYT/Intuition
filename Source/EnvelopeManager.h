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

    void noteOn(int noteNumber);
    void noteOff(int noteNumber);

    void process(int numSamples);
    float getEnvValue(size_t index);

private:
    juce::AudioProcessorValueTreeState& parameters;
    std::array<GlobalEnvelope, 3> envs;

    std::array<float, 3> lastValues;

    std::unordered_set<int> activeNotes;
};
