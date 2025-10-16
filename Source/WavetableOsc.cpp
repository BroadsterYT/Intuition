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

void WavetableOsc::setBank(WavetableBank* newBank) {
    bank = newBank;
}

void WavetableOsc::setSampleRate(double newRate) {
    sampleRate = newRate;
    morphSmooth.reset(sampleRate, 0.01);
}

void WavetableOsc::setFrequency(
    float frequency,
    int octave,
    int semitones,
    int fineCents,
    int detuneCents) {
    if (bank->size() == 0 || bank->getWavetable(0).getNumSamples() == 0) return;

    float totalCents = (octave * 1200.0f) + (semitones * 100.0f) + fineCents + detuneCents;
    float pitchMult = std::pow(2.0f, (float)totalCents / 1200.0f);

    float finalFreq = frequency * pitchMult;
    phaseIncrement = finalFreq / (float)sampleRate;
}

void WavetableOsc::setMorph(float alpha) {
    morphSmooth.setTargetValue(alpha);
}

void WavetableOsc::resetPhase() {
    phase = 0.0f;
}

float WavetableOsc::getSample() {
    if (!bank || bank->getWavetable(0).getNumSamples() <= 0) return 0.0f;

    //int tableSize = wavetable.getNumSamples();
    int tableSize = bank->getWavetable(0).getNumSamples();

    float idx = phase * tableSize;  // Unnormalized phase from 0-1 to 0-tableSize
    int i1 = (int)idx;              // Unnormalized phase (integer part)
    float frac = idx - i1;        // Unnormalized phase (fractional part)
    int i2 = (i1 + 1) % tableSize;  // The next sample in the wavetable, if i1 is the last sample, it wraps back to beginning

    // Assuming all wavetables are the same size!
    float alpha = morphSmooth.getNextValue();
    float widx = juce::jlimit(0.0f, (float)(bank->size() - 1), alpha * (bank->size() - 1));
    int wi1 = (int)widx;
    float wFrac = widx - wi1;

    const float* tableA = bank->getWavetable(wi1).getReadPointer(0);
    const float* tableB = bank->getWavetable((wi1 + 1) % bank->size()).getReadPointer(0);

    float sampleA = tableA[i1] + frac * (tableA[i2] - tableA[i1]);
    float sampleB = tableB[i1] + frac * (tableB[i2] - tableB[i1]);

    // Cosine crossfade for smooth morphing
    float t = 0.5f - 0.5f * std::cos(wFrac * juce::MathConstants<float>::pi);
    float sample = (1.0f - t) * sampleA + t * sampleB;

    phase += phaseIncrement;
    while (phase >= 1.0f) {
        phase -= 1.0f;
    }

    return sample;
}