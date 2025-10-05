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
#include "UnisonOsc.h"
#include "WavetableBank.h"


class UnisonWavetableVoice : public juce::SynthesiserVoice {
public:
    UnisonWavetableVoice(
        juce::AudioProcessorValueTreeState& vts,
        WavetableBank* bankToUse1,
        WavetableBank* bankToUse2,
        WavetableBank* bankToUse3,
        WavetableBank* bankToUse4
    );

    //void setWavetable(WavetableBank& bankToUse);

    int getUnison();
    UnisonOsc& getOsc(int index);

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
    juce::AudioProcessorValueTreeState& parameters;

    int unison = 4;
    std::vector<UnisonOsc> oscs;
    WavetableBank* bank1 = nullptr;
    WavetableBank* bank2 = nullptr;
    WavetableBank* bank3 = nullptr;
    WavetableBank* bank4 = nullptr;
    std::vector<WavetableBank*> banks = { bank1, bank2, bank3, bank4 };

    void setUnison();

    juce::ADSR adsr;
    float currentFreq = 440.0f;

    float level = 1.0f;
    float detuneRange = 100.0f;  // Measured in cents
    std::vector<float> detuneOffsets; // The pitch offsets for each oscillator
};