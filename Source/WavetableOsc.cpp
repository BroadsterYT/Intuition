/*
  ==============================================================================

    WavetableOsc.cpp
    Created: 21 Sep 2025 10:22:39pm
    Author:  BroDe

  ==============================================================================
*/

#include "WavetableOsc.h"


void WavetableOsc::setParameters(juce::AudioProcessorValueTreeState* vts) {
    parameters = vts;
}

void WavetableOsc::setBank(WavetableBank& newBank) {
    bank = newBank;
}

void WavetableOsc::setSampleRate(double newRate) {
    sampleRate = newRate;
}

void WavetableOsc::setFrequency(float frequency, int detuneCents) {
    //if (wavetable.getNumSamples() > 0) {
    if (bank.getWavetable(0).getNumSamples() > 0) {
        float detunedMult = std::pow(2.0f, (float)detuneCents / 1200.0f);
        float detunedFreq = frequency * detunedMult;

        phaseIncrement = detunedFreq / (float)sampleRate;
    }
}

float WavetableOsc::getSample() {
    //if (wavetable.getNumSamples() == 0) return 0.0f;
    if (bank.getWavetable(0).getNumSamples() <= 0) return 0.0f;

    //int tableSize = wavetable.getNumSamples();
    int tableSize = bank.getWavetable(0).getNumSamples();

    float idx = phase * tableSize;  // Unnormalized phase from 0-1 to 0-tableSize
    int i1 = (int)idx;              // Unnormalized phase (integer part)
    float frac = idx - i1;        // Unnormalized phase (fractional part)
    int i2 = (i1 + 1) % tableSize;  // The next sample in the wavetable, if i1 is the last sample, it wraps back to beginning

    // Assuming all wavetables are the same size!
    float alpha = *parameters->getRawParameterValue("A_MORPH");
    float widx = alpha * (bank.size() - 1);
    int wi1 = (int)widx;
    float wFrac = widx - wi1;
    int wi2 = (wi1 + 1) % bank.size();

    const float* tableA = bank.getWavetable(wi1).getReadPointer(0);
    const float* tableB = bank.getWavetable(wi2).getReadPointer(0);

    float sampleA = tableA[i1] + frac * (tableA[i2] - tableA[i1]); // Proper sample taken from wavetable, if phase was not exact integer
    float sampleB = tableB[i1] + frac * (tableB[i2] - tableB[i1]); 
    float sample = (1 - wFrac) * sampleA + wFrac * sampleB;

    phase += phaseIncrement;
    while (phase >= 1.0f) {
        phase -= 1.0f;
    }

    return sample;
}