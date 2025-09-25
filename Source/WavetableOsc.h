/*
  ==============================================================================

    WavetableOsc.h
    Created: 21 Sep 2025 10:22:30pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class WavetableOsc {
public:
    void setWavetable(const juce::AudioBuffer<float>& table);
    void setSampleRate(double newRate);
    void setFrequency(float frequency, int detuneCents);
    float getSample();

private:
    juce::AudioBuffer<float> wavetable;
    double sampleRate = 44100.0;

    float phase = 0.0f;  // Phase goes from 0 to 1, 0 being the beginning of a single wave and 1 being the end
    float phaseIncrement = 0.0f;  // Cycles per sample
};