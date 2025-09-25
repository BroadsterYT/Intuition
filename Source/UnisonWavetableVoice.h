/*
  ==============================================================================

    UnisonWavetableVoice.h
    Created: 24 Sep 2025 3:33:17pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "WavetableOsc.h"


class UnisonWavetableVoice : public juce::SynthesiserVoice {
public:
    UnisonWavetableVoice(const juce::AudioBuffer<float>& tableToUse);

    int getUnison();
    void setUnison(int newUnison);

    float getDetuneRange();
    void setDetuneRange(float range);
    void updateOscDetunes();

    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*pitchWheel*/) override;
    void stopNote(float /*velocity*/, bool allowTailOff) override;

    void pitchWheelMoved(int) override;
    void controllerMoved(int, int) override;

    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    void setEnvelopeParams(const juce::ADSR::Parameters& params);

private:
    int unison = 1;
    std::vector<WavetableOsc> oscs;
    juce::AudioBuffer<float> wavetable;

    juce::ADSR adsr;
    float currentFreq = 440.0f;

    float level = 1.0f;
    float detuneRange = 100.0f;  // Measured in cents
    std::vector<float> detuneOffsets; // The pitch offsets for each oscillator
};