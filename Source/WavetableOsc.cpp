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

void WavetableOsc::setBank(WavetableBank& bank) {
    this->bank = bank;
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

    const float* tableA = bank.getWavetable(0).getReadPointer(0);
    const float* tableB = bank.getWavetable(bank.size() - 1).getReadPointer(0);
    
    // Assuming both wavetables are the same size!
    float alpha = *parameters->getRawParameterValue("A_MORPH");
    float sampleA = tableA[i1] + frac * (tableA[i2] - tableA[i1]); // Proper sample taken from wavetable, if phase was not exact integer
    float sampleB = tableB[i1] + frac * (tableB[i2] - tableB[i1]); // Proper sample taken from wavetable, if phase was not exact integer
    float sample = (1 - alpha) * sampleA + alpha * sampleB;

    phase += phaseIncrement;
    while (phase >= 1.0f) {
        phase -= 1.0f;
    }

    return sample;
}