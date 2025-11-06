/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SineWaveSound.h"
#include "UnisonVoice.h"
#include "LFOShape.h"
#include "WavetableBank.h"
#include "ModMatrix.h"
#include "ItnContext.h"
#include "EnvelopeManager.h"
#include "ReverbModule.h"
#include "AIManager.h"


//==============================================================================
/**
*/
class IntuitionAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    IntuitionAudioProcessor();
    ~IntuitionAudioProcessor() override;

    juce::Synthesiser synth;

    juce::AudioProcessorValueTreeState parameters;
    ModMatrix modMatrix;
    juce::MidiKeyboardState keyboardState;

    WavetableBank bank1, bank2, bank3, bank4;

    LFOShape lfoShape1;
    LFOShape lfoShape2;
    LFOShape lfoShape3;

    EnvelopeManager envManager;

    ItnContext context;

    void resetSynths();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    float currentBPM = 60.0;

    float lfoRate1 = 1.0f;
    float lfoRate2 = 1.0f;
    float lfoRate3 = 1.0f;
    float lfoPhase1 = 0.0f;
    float lfoPhase2 = 0.0f;
    float lfoPhase3 = 0.0f;
    float lfoValue1 = 0.0f;
    float lfoValue2 = 0.0f;
    float lfoValue3 = 0.0f;

    float envValueOsc = 0.0f;
    float envValue1 = 0.0f;
    float envValue2 = 0.0f;
    float envValue3 = 0.0f;

    ReverbModule reverbModule;

    float getDivisionFloat(int syncDiv);
    void calculateLFOFrequency(
        const juce::String modeName,
        const juce::String rateName,
        const juce::String syncDivName,
        float& rateVal
    );
    void calculateLFOPhase(
        LFOShape& shape,
        float& phase,
        const juce::String modeName,
        const juce::String syncDivName,
        const juce::String rateName,
        float& lfoValue,
        float sampleRate,
        int numSamples
    );

    void setCurrentBPM();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IntuitionAudioProcessor)
};
