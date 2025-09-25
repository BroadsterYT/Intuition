/*
  ==============================================================================

    WavetableOsc.cpp
    Created: 21 Sep 2025 10:22:39pm
    Author:  BroDe

  ==============================================================================
*/

#include "WavetableOsc.h"


void WavetableOsc::setWavetable(const juce::AudioBuffer<float>& table) {
    wavetable.makeCopyOf(table);
}

void WavetableOsc::setSampleRate(double newRate) {
    sampleRate = newRate;
}

void WavetableOsc::setFrequency(float frequency, int detuneCents) {
    if (wavetable.getNumSamples() > 0) {
        float detunedMult = std::pow(2.0f, (float)detuneCents / 1200.0f);
        float detunedFreq = frequency * detunedMult;

        phaseIncrement = detunedFreq / (float)sampleRate;
    }
}

float WavetableOsc::getSample() {
    if (wavetable.getNumSamples() == 0) return 0.0f;

    int tableSize = wavetable.getNumSamples();

    float idx = phase * tableSize;  // Unnormalized phase from 0-1 to 0-tableSize
    int i1 = (int)idx;              // Unnormalized phase (integer part)
    float frac = idx - i1;        // Unnormalized phase (fractional part)

    int i2 = (i1 + 1) % tableSize;  // The next sample in the wavetable, if i1 is the last sample, it wraps back to beginning

    const float* tableData = wavetable.getReadPointer(0);  // The wavetable as an array
    float sample = tableData[i1] + frac * (tableData[i2] - tableData[i1]); // Proper sample taken from wavetable, if phase was not exact integer

    phase += phaseIncrement;
    while (phase >= 1.0f) {
        phase -= 1.0f;
    }

    return sample;
}