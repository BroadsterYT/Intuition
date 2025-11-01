/*
  ==============================================================================

    WavetableOsc.h
    Created: 21 Sep 2025 10:22:30pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "WavetableBank.h"


class WavetableOsc {
public:
    void setParameters(juce::AudioProcessorValueTreeState* vts);
    void setBank(WavetableBank* newBank);

    void setSampleRate(double newRate);
    void setFrequency(
        float frequency,
        int octave,
        int semitone,
        int fineCents,
        int detuneCents
    );
    void setMorph(float alpha);

    void setPhaseOffset(float newOffset);
    void resetPhase();
    float getSample();

private:
    juce::AudioProcessorValueTreeState* parameters;
    WavetableBank* bank = nullptr;
    double sampleRate = 44100.0;

    juce::SmoothedValue<float> morphSmooth;
    float lastAlpha = 0.0f;

    float phase = 0.0f;  // Phase goes from 0 to 1, 0 being the beginning of a single wave and 1 being the end
    float phaseOffset = 0.0f;
    float phaseIncrement = 0.0f;  // Cycles per sample
};