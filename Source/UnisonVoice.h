/*
  ==============================================================================

    UnisonVoice.h
    Created: 24 Sep 2025 3:33:17pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "WavetableOsc.h"
#include "UnisonOsc.h"
#include "WavetableBank.h"
#include "ModMatrix.h"
#include "EnvelopeManager.h"


class UnisonVoice : public juce::SynthesiserVoice {
public:
    UnisonVoice(
        juce::AudioProcessorValueTreeState& vts,
        ModMatrix& matrix,
        EnvelopeManager& envManager,
        WavetableBank* bankToUse1,
        WavetableBank* bankToUse2,
        WavetableBank* bankToUse3,
        WavetableBank* bankToUse4
    );

    UnisonOsc& getOscA();
    UnisonOsc& getOscB();
    UnisonOsc& getOscC();
    UnisonOsc& getOscD();

    void updatePitch();

    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*pitchWheel*/) override;
    void stopNote(float /*velocity*/, bool allowTailOff) override;

    void pitchWheelMoved(int) override;
    void controllerMoved(int, int) override;

    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    /// <summary>
    /// Sets the filter cutoff frequency. Currently reads FILTER_CUTOFF parameter
    /// </summary>
    void setFilterCutoff(float cutoff);
    /// <summary>
    /// Sets the resonance of the filter. Currently uses FILTER_RESONANCE parameter.
    /// </summary>
    void setFilterResonance(float res);
    void setFilterType(int type);

    void setEnvelopeParams(const juce::ADSR::Parameters& params);

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix& modMatrix;
    EnvelopeManager& envManager;

    WavetableBank* bank1 = nullptr;
    WavetableBank* bank2 = nullptr;
    WavetableBank* bank3 = nullptr;
    WavetableBank* bank4 = nullptr;

    UnisonOsc oscA, oscB, oscC, oscD;

    juce::dsp::StateVariableTPTFilter<float> filter;
    int filterPreparedChannels = 0;

    juce::ADSR adsr;  // Only used to control voice on/off
    int currentMidiNote = -1;
    float currentFreq = 440.0f;

    float level = 1.0f;
    void prepareFilter(double sampleRate, int samplesPerBlock, int numChannels);
};