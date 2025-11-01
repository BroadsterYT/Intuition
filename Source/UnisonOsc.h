/*
  ==============================================================================

    UnisonOsc.h
    Created: 4 Oct 2025 11:33:46pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include "WavetableOsc.h"
#include "WavetableBank.h"

/// <summary>
/// An oscillator of oscillators
/// </summary>
class UnisonOsc {
public:
    UnisonOsc();

    void setParameters(juce::AudioProcessorValueTreeState* vts);
    void setBank(WavetableBank* newBank);
    
    void setSampleRate(double newRate);
    void setFrequency(
        float frequency,
        int octave,
        int semitones,
        int cents,
        float detuneCents
    );
    void setCurrentFrequency(float freq);
    
    void setUnison(int newUnison);
    void setMorph(float alpha);
    void setRandomPhaseOffsets();

    void setDetuneRange(float range);
    void updateOscDetunes();

    void resetPhase();
    std::pair<float, float> getSample();

private:
    std::vector<WavetableOsc> oscillators;
    WavetableBank* bank = nullptr;
    juce::AudioProcessorValueTreeState* parameters = nullptr;

    double sampleRate = 44100.0;
    int unison = 1;
    float stereoWidth = 0.5f;

    int octave = 0;
    int coarse = 0;
    int fine = 0;
    
    float detuneRange = 100.0f;
    std::vector<float> detuneOffsets;

    float currentFreq = 440.0f;
    float phase = 0.0f;
    float phaseIncrement = 0.0f;
};