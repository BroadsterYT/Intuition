/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "WavetableHelper.h"

//==============================================================================
IntuitionAudioProcessor::IntuitionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
#endif
    // Setting parameters
    parameters(*this, nullptr, "PARAMETERS", {
        std::make_unique<juce::AudioParameterBool>("DEFAULT", "default", false),  // For ButtonComponents without a designated parameter
        std::make_unique<juce::AudioParameterFloat>("MASTER", "Master Volume", 0.0f, 1.0f, 0.75f),

        //================ Envelopes =================//
        std::make_unique<juce::AudioParameterFloat>("ENV_OSC_ATTACK", "Envelope 1 Attack", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("ENV_OSC_DECAY", "Envelope 1 Decay", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("ENV_OSC_SUSTAIN", "Envelope 1 Sustain", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("ENV_OSC_RELEASE", "Envelope 1 Release", 0.0f, 1.0f, 0.0f),

        std::make_unique<juce::AudioParameterFloat>("ENV1_ATTACK", "Envelope 1 Attack", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("ENV1_DECAY", "Envelope 1 Decay", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("ENV1_SUSTAIN", "Envelope 1 Sustain", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("ENV1_RELEASE", "Envelope 1 Release", 0.0f, 1.0f, 0.0f),

        std::make_unique<juce::AudioParameterFloat>("ENV2_ATTACK", "Envelope 2 Attack", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("ENV2_DECAY", "Envelope 2 Decay", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("ENV2_SUSTAIN", "Envelope 2 Sustain", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("ENV2_RELEASE", "Envelope 2 Release", 0.0f, 1.0f, 0.0f),

        std::make_unique<juce::AudioParameterFloat>("ENV3_ATTACK", "Envelope 3 Attack", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("ENV3_DECAY", "Envelope 3 Decay", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("ENV3_SUSTAIN", "Envelope 3 Sustain", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("ENV3_RELEASE", "Envelope 3 Release", 0.0f, 1.0f, 0.0f),

        //=============== Oscillators ================//
        std::make_unique<juce::AudioParameterBool>("A_TOGGLE", "A Toggle", true),
        std::make_unique<juce::AudioParameterFloat>("A_VOLUME", "A Volume", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("A_PAN", "A Panning", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterInt>("A_UNISON", "A Unison", 1, 8, 1),
        std::make_unique<juce::AudioParameterFloat>("A_DETUNE", "A Detune", 0, 100, 0),
        std::make_unique<juce::AudioParameterFloat>("A_MORPH", "A Morph", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterInt>("A_OCTAVE", "A Octave", -4, 4, 0),
        std::make_unique<juce::AudioParameterInt>("A_COARSE", "A Coarse Pitch", -12, 12, 0),
        std::make_unique<juce::AudioParameterInt>("A_FINE", "A Fine Pitch", -100, 100, 0),

        std::make_unique<juce::AudioParameterBool>("B_TOGGLE", "B Toggle", false),
        std::make_unique<juce::AudioParameterFloat>("B_VOLUME", "B Volume", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("B_PAN", "B Panning", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterInt>("B_UNISON", "B Unison", 1, 8, 1),
        std::make_unique<juce::AudioParameterFloat>("B_DETUNE", "B Detune", 0, 100, 0),
        std::make_unique<juce::AudioParameterFloat>("B_MORPH", "B Morph", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterInt>("B_OCTAVE", "B Octave", -4, 4, 0),
        std::make_unique<juce::AudioParameterInt>("B_COARSE", "B Coarse Pitch", -12, 12, 0),
        std::make_unique<juce::AudioParameterInt>("B_FINE", "B Fine Pitch", -100, 100, 0),

        std::make_unique<juce::AudioParameterBool>("C_TOGGLE", "C Toggle", false),
        std::make_unique<juce::AudioParameterFloat>("C_VOLUME", "C Volume", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("C_PAN", "C Panning", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterInt>("C_UNISON", "C Unison", 1, 8, 1),
        std::make_unique<juce::AudioParameterFloat>("C_DETUNE", "C Detune", 0, 100, 0),
        std::make_unique<juce::AudioParameterFloat>("C_MORPH", "C Morph", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterInt>("C_OCTAVE", "C Octave", -4, 4, 0),
        std::make_unique<juce::AudioParameterInt>("C_COARSE", "C Coarse Pitch", -12, 12, 0),
        std::make_unique<juce::AudioParameterInt>("C_FINE", "C Fine Pitch", -100, 100, 0),

        std::make_unique<juce::AudioParameterBool>("D_TOGGLE", "D Toggle", false),
        std::make_unique<juce::AudioParameterFloat>("D_VOLUME", "D Volume", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("D_PAN", "D Panning", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterInt>("D_UNISON", "D Unison", 1, 8, 1),
        std::make_unique<juce::AudioParameterFloat>("D_DETUNE", "D Detune", 0, 100, 0),
        std::make_unique<juce::AudioParameterFloat>("D_MORPH", "D Morph", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterInt>("D_OCTAVE", "D Octave", -4, 4, 0),
        std::make_unique<juce::AudioParameterInt>("D_COARSE", "D Coarse Pitch", -12, 12, 0),
        std::make_unique<juce::AudioParameterInt>("D_FINE", "D Fine Pitch", -100, 100, 0),

        //=============== LFOs ===============//
        std::make_unique<juce::AudioParameterChoice>("LFO1_MODE", "LFO 1 Mode", juce::StringArray{"Free", "Synced"}, 0),
        std::make_unique<juce::AudioParameterChoice>("LFO1_SYNC_DIV", "LFO 1 Sync Division", juce::StringArray{"4 bars", "2 bars", "1/1", "1/2", "1/4", "1/8", "1/16", "1/32", "1/64"}, 2),
        std::make_unique<juce::AudioParameterFloat>("LFO1_RATE", "LFO 1 Rate", 0.01f, 30.0f, 1.0f),

        std::make_unique<juce::AudioParameterChoice>("LFO2_MODE", "LFO 2 Mode", juce::StringArray{"Free", "Synced"}, 0),
        std::make_unique<juce::AudioParameterChoice>("LFO2_SYNC_DIV", "LFO 2 Sync Division", juce::StringArray{"4 bars", "2 bars", "1/1", "1/2", "1/4", "1/8", "1/16", "1/32", "1/64"}, 2),
        std::make_unique<juce::AudioParameterFloat>("LFO2_RATE", "LFO 2 Rate", 0.01f, 30.0f, 1.0f),

        std::make_unique<juce::AudioParameterChoice>("LFO3_MODE", "LFO 3 Mode", juce::StringArray{"Free", "Synced"}, 0),
        std::make_unique<juce::AudioParameterChoice>("LFO3_SYNC_DIV", "LFO 3 Sync Division", juce::StringArray{"4 bars", "2 bars", "1/1", "1/2", "1/4", "1/8", "1/16", "1/32", "1/64"}, 2),
        std::make_unique<juce::AudioParameterFloat>("LFO3_RATE", "LFO 3 Rate", 0.01f, 30.0f, 1.0f),

        //============== Filter ==============//
        std::make_unique<juce::AudioParameterBool>("A_FILTER_SEND", "A Filter Send", false),
        std::make_unique<juce::AudioParameterBool>("B_FILTER_SEND", "B Filter Send", false),
        std::make_unique<juce::AudioParameterBool>("C_FILTER_SEND", "C Filter Send", false),
        std::make_unique<juce::AudioParameterBool>("D_FILTER_SEND", "D Filter Send", false),

        std::make_unique<juce::AudioParameterFloat>("FILTER_CUTOFF", "Filter Cutoff Frequency", 20.0f, 20000.0f, 1000.0f),
        std::make_unique<juce::AudioParameterFloat>("FILTER_RESONANCE", "Filter Cutoff Frequency", 0.01f, 1.0f, 0.7f),
        std::make_unique<juce::AudioParameterChoice>("FILTER_TYPE", "Filter Type", juce::StringArray{"Low", "High", "Band"}, 0),

        //============== Reverb ==============//
        std::make_unique<juce::AudioParameterBool>("REVERB_TOGGLE", "Reverb Toggle", false),
        std::make_unique<juce::AudioParameterFloat>("REVERB_DAMPING", "Damping", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>("REVERB_ROOM_SIZE", "Room Size", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>("REVERB_WIDTH", "Width", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("REVERB_DRY_LEVEL", "Reverb Dry Level", 0.0f, 1.0f, 0.7f),
        std::make_unique<juce::AudioParameterFloat>("REVERB_WET_LEVEL", "Reverb Wet Level", 0.0f, 1.0f, 0.3f),

        //============== Delay ===============//
        std::make_unique<juce::AudioParameterBool>("DELAY_TOGGLE", "Delay Toggle", false),
        std::make_unique<juce::AudioParameterFloat>("DELAY_TIME_MS", "Delay Time", 0.0f, 2000.0f, 1000.0f),
        std::make_unique<juce::AudioParameterFloat>("DELAY_FEEDBACK", "Delay Feedback", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>("DELAY_CUTOFF", "Delay Cutoff Frequency", 20.0f, 20000.0f, 20000.0f),
        std::make_unique<juce::AudioParameterFloat>("DELAY_DRY_LEVEL", "Delay Dry Level", 0.0f, 1.0f, 0.7f),
        std::make_unique<juce::AudioParameterFloat>("DELAY_WET_LEVEL", "Delay Wet Level", 0.0f, 1.0f, 0.3f),

        //============== Chorus ==============//
        std::make_unique<juce::AudioParameterBool>("CHORUS_TOGGLE", "Chorus Toggle", false),
        std::make_unique<juce::AudioParameterFloat>("CHORUS_RATE", "Chorus Rate", 0.05f, 5.0f, 0.3f),
        std::make_unique<juce::AudioParameterFloat>("CHORUS_DEPTH", "Chorus Depth", 0.5f, 8.0f, 3.0f),
        std::make_unique<juce::AudioParameterFloat>("CHORUS_WIDTH", "Chorus Width", 0.0f, 1.0f, 7.0f),
        std::make_unique<juce::AudioParameterFloat>("CHORUS_DRY_LEVEL", "Chorus Dry Level", 0.0f, 1.0f, 0.7f),
        std::make_unique<juce::AudioParameterFloat>("CHORUS_WET_LEVEL", "Chorus Wet Level", 0.0f, 1.0f, 0.3f),
    }),
    context(
        this,
        parameters,
        &modMatrix,
        envManager,

        bank1,
        bank2,
        bank3,
        bank4,

        lfoShape1,
        lfoShape2,
        lfoShape3,
        &lfoPhase1,
        &lfoPhase2,
        &lfoPhase3
    ),
    envManager(parameters),
    reverbModule(parameters, &modMatrix),
    delayModule(parameters, &modMatrix),
    chorusModule(parameters, &modMatrix)
{
    parameters.state = juce::ValueTree("PARAMETERS");
    initializeUserDirectory();

    const char* wav = BinaryData::AKWF_sin_wav;
    int wavSize = BinaryData::AKWF_sin_wavSize;
    if (bank1.size() == 0) {
        bank1.addWavetable(wav, wavSize);
    }
    if (bank2.size() == 0) {
        bank2.addWavetable(wav, wavSize);
    }
    if (bank3.size() == 0) {
        bank3.addWavetable(wav, wavSize);
    }
    if (bank4.size() == 0) {
        bank4.addWavetable(wav, wavSize);
    }

    resetSynths();

    //========== ModMatrix Setup ==========//
    //===== Sources
    ModSource* lfoSource1 = modMatrix.addSource("LFO1");
    lfoSource1->setValuePtr(&lfoValue1);
    ModSource* lfoSource2 = modMatrix.addSource("LFO2");
    lfoSource2->setValuePtr(&lfoValue2);
    ModSource* lfoSource3 = modMatrix.addSource("LFO3");
    lfoSource3->setValuePtr(&lfoValue3);

    ModSource* envSourceA = modMatrix.addSource("ENV_Osc");
    envSourceA->setValuePtr(&envValueOsc);

    ModSource* envSource1 = modMatrix.addSource("ENV1");
    envSource1->setValuePtr(&envValue1);
    ModSource* envSource2 = modMatrix.addSource("ENV2");
    envSource2->setValuePtr(&envValue2);
    ModSource* envSource3 = modMatrix.addSource("ENV3");
    envSource3->setValuePtr(&envValue3);

    //===== Destinations
    ModDestination* aOctDest = modMatrix.addDestination("A_OCTAVE");
    ModDestination* aCoarseDest = modMatrix.addDestination("A_COARSE");
    ModDestination* aFineDest = modMatrix.addDestination("A_FINE");
    ModDestination* aMorphDest = modMatrix.addDestination("A_MORPH");
    ModDestination* aDetuneDest = modMatrix.addDestination("A_DETUNE");
    ModDestination* aVolumeDest = modMatrix.addDestination("A_VOLUME");
    ModDestination* aPanDest = modMatrix.addDestination("A_PAN");
    
    aOctDest->setBasePtr(parameters.getRawParameterValue("A_OCTAVE"));
    aOctDest->setMinRange(-4);
    aOctDest->setMaxRange(4);
    aCoarseDest->setBasePtr(parameters.getRawParameterValue("A_COARSE"));
    aCoarseDest->setMinRange(-12);
    aCoarseDest->setMaxRange(12);
    aFineDest->setBasePtr(parameters.getRawParameterValue("A_FINE"));
    aFineDest->setMinRange(-100);
    aFineDest->setMaxRange(100);
    aMorphDest->setBasePtr(parameters.getRawParameterValue("A_MORPH"));
    aDetuneDest->setBasePtr(parameters.getRawParameterValue("A_DETUNE"));
    aDetuneDest->setMinRange(0.0f);
    aDetuneDest->setMaxRange(100.0f);
    aVolumeDest->setBasePtr(parameters.getRawParameterValue("A_VOLUME"));
    aPanDest->setBasePtr(parameters.getRawParameterValue("A_PAN"));

    ModDestination* bOctDest = modMatrix.addDestination("B_OCTAVE");
    ModDestination* bCoarseDest = modMatrix.addDestination("B_COARSE");
    ModDestination* bFineDest = modMatrix.addDestination("B_FINE");
    ModDestination* bMorphDest = modMatrix.addDestination("B_MORPH");
    ModDestination* bDetuneDest = modMatrix.addDestination("B_DETUNE");
    ModDestination* bVolumeDest = modMatrix.addDestination("B_VOLUME");
    ModDestination* bPanDest = modMatrix.addDestination("B_PAN");

    bOctDest->setBasePtr(parameters.getRawParameterValue("B_OCTAVE"));
    bOctDest->setMinRange(-4);
    bOctDest->setMaxRange(4);
    bCoarseDest->setBasePtr(parameters.getRawParameterValue("B_COARSE"));
    bCoarseDest->setMinRange(-12);
    bCoarseDest->setMaxRange(12);
    bFineDest->setBasePtr(parameters.getRawParameterValue("B_FINE"));
    bFineDest->setMinRange(-100);
    bFineDest->setMaxRange(100);
    bMorphDest->setBasePtr(parameters.getRawParameterValue("B_MORPH"));
    bDetuneDest->setBasePtr(parameters.getRawParameterValue("B_DETUNE"));
    bDetuneDest->setMinRange(0.0f);
    bDetuneDest->setMaxRange(100.0f);
    bVolumeDest->setBasePtr(parameters.getRawParameterValue("B_VOLUME"));
    bPanDest->setBasePtr(parameters.getRawParameterValue("B_PAN"));

    ModDestination* cOctDest = modMatrix.addDestination("C_OCTAVE");
    ModDestination* cCoarseDest = modMatrix.addDestination("C_COARSE");
    ModDestination* cFineDest = modMatrix.addDestination("C_FINE");
    ModDestination* cMorphDest = modMatrix.addDestination("C_MORPH");
    ModDestination* cDetuneDest = modMatrix.addDestination("C_DETUNE");
    ModDestination* cVolumeDest = modMatrix.addDestination("C_VOLUME");
    ModDestination* cPanDest = modMatrix.addDestination("C_PAN");

    cOctDest->setBasePtr(parameters.getRawParameterValue("C_OCTAVE"));
    cOctDest->setMinRange(-4);
    cOctDest->setMaxRange(4);
    cCoarseDest->setBasePtr(parameters.getRawParameterValue("C_COARSE"));
    cCoarseDest->setMinRange(-12);
    cCoarseDest->setMaxRange(12);
    cFineDest->setBasePtr(parameters.getRawParameterValue("C_FINE"));
    cFineDest->setMinRange(-100);
    cFineDest->setMaxRange(100);
    cMorphDest->setBasePtr(parameters.getRawParameterValue("C_MORPH"));
    cDetuneDest->setBasePtr(parameters.getRawParameterValue("C_DETUNE"));
    cDetuneDest->setMinRange(0.0f);
    cDetuneDest->setMaxRange(100.0f);
    cVolumeDest->setBasePtr(parameters.getRawParameterValue("C_VOLUME"));
    cPanDest->setBasePtr(parameters.getRawParameterValue("C_PAN"));

    ModDestination* dOctDest = modMatrix.addDestination("D_OCTAVE");
    ModDestination* dCoarseDest = modMatrix.addDestination("D_COARSE");
    ModDestination* dFineDest = modMatrix.addDestination("D_FINE");
    ModDestination* dMorphDest = modMatrix.addDestination("D_MORPH");
    ModDestination* dDetuneDest = modMatrix.addDestination("D_DETUNE");
    ModDestination* dVolumeDest = modMatrix.addDestination("D_VOLUME");
    ModDestination* dPanDest = modMatrix.addDestination("D_PAN");

    dOctDest->setBasePtr(parameters.getRawParameterValue("D_OCTAVE"));
    dOctDest->setMinRange(-4);
    dOctDest->setMaxRange(4);
    dCoarseDest->setBasePtr(parameters.getRawParameterValue("D_COARSE"));
    dCoarseDest->setMinRange(-12);
    dCoarseDest->setMaxRange(12);
    dFineDest->setBasePtr(parameters.getRawParameterValue("D_FINE"));
    dFineDest->setMinRange(-100);
    dFineDest->setMaxRange(100);
    dMorphDest->setBasePtr(parameters.getRawParameterValue("D_MORPH"));
    dDetuneDest->setBasePtr(parameters.getRawParameterValue("D_DETUNE"));
    dDetuneDest->setMinRange(0.0f);
    dDetuneDest->setMaxRange(100.0f);
    dVolumeDest->setBasePtr(parameters.getRawParameterValue("D_VOLUME"));
    dPanDest->setBasePtr(parameters.getRawParameterValue("D_PAN"));

    ModDestination* filterCutoffDest = modMatrix.addDestination("FILTER_CUTOFF");
    ModDestination* filterResonanceDest = modMatrix.addDestination("FILTER_RESONANCE");

    filterCutoffDest->setBasePtr(parameters.getRawParameterValue("FILTER_CUTOFF"));
    filterCutoffDest->setMinRange(20.0f);
    filterCutoffDest->setMaxRange(20000.0f);
    filterResonanceDest->setBasePtr(parameters.getRawParameterValue("FILTER_RESONANCE"));
    filterResonanceDest->setMinRange(0.01f);
    filterResonanceDest->setMaxRange(1.0f);

    //=== Reverb
    ModDestination* rvbDampingDest = modMatrix.addDestination("REVERB_DAMPING");
    ModDestination* rvbRoomSizeDest = modMatrix.addDestination("REVERB_ROOM_SIZE");
    ModDestination* rvbWidthDest = modMatrix.addDestination("REVERB_WIDTH");
    ModDestination* rvbDryLevelDest = modMatrix.addDestination("REVERB_DRY_LEVEL");
    ModDestination* rvbWetLevelDest = modMatrix.addDestination("REVERB_WET_LEVEL");

    rvbDampingDest->setBasePtr(parameters.getRawParameterValue("REVERB_DAMPING"));
    rvbRoomSizeDest->setBasePtr(parameters.getRawParameterValue("REVERB_ROOM_SIZE"));
    rvbWidthDest->setBasePtr(parameters.getRawParameterValue("REVERB_WIDTH"));
    rvbDryLevelDest->setBasePtr(parameters.getRawParameterValue("REVERB_DRY_LEVEL"));
    rvbWetLevelDest->setBasePtr(parameters.getRawParameterValue("REVERB_WET_LEVEL"));

    //=== Delay
    ModDestination* dlyTimeMsDest = modMatrix.addDestination("DELAY_TIME_MS");
    ModDestination* dlyFeedbackDest = modMatrix.addDestination("DELAY_FEEDBACK");
    ModDestination* dlyCutoffDest = modMatrix.addDestination("DELAY_CUTOFF");
    ModDestination* dlyDryDest = modMatrix.addDestination("DELAY_DRY_LEVEL");
    ModDestination* dlyWetDest = modMatrix.addDestination("DELAY_WET_LEVEL");

    dlyTimeMsDest->setBasePtr(parameters.getRawParameterValue("DELAY_TIME_MS"));
    dlyTimeMsDest->setMaxRange(2000);
    dlyFeedbackDest->setBasePtr(parameters.getRawParameterValue("DELAY_FEEDBACK"));
    dlyCutoffDest->setBasePtr(parameters.getRawParameterValue("DELAY_CUTOFF"));
    dlyCutoffDest->setMinRange(20.0f);
    dlyCutoffDest->setMaxRange(20000.0f);
    dlyDryDest->setBasePtr(parameters.getRawParameterValue("DELAY_DRY_LEVEL"));
    dlyWetDest->setBasePtr(parameters.getRawParameterValue("DELAY_WET_LEVEL"));

    //=== Chorus
    ModDestination* chsRateDest = modMatrix.addDestination("CHORUS_RATE");
    ModDestination* chsDepthDest = modMatrix.addDestination("CHORUS_DEPTH");
    ModDestination* chsWidthDest = modMatrix.addDestination("CHORUS_WIDTH");
    ModDestination* chsDryDest = modMatrix.addDestination("CHORUS_DRY_LEVEL");
    ModDestination* chsWetDest = modMatrix.addDestination("CHORUS_WET_LEVEL");

    chsRateDest->setBasePtr(parameters.getRawParameterValue("CHORUS_RATE"));
    chsRateDest->setMinRange(0.05f);
    chsRateDest->setMaxRange(5.0f);
    chsDepthDest->setBasePtr(parameters.getRawParameterValue("CHORUS_DEPTH"));
    chsDepthDest->setMinRange(0.5f);
    chsDepthDest->setMaxRange(8.0f);
    chsWidthDest->setBasePtr(parameters.getRawParameterValue("CHORUS_WIDTH"));
    chsDryDest->setBasePtr(parameters.getRawParameterValue("CHORUS_DRY_LEVEL"));
    chsWetDest->setBasePtr(parameters.getRawParameterValue("CHORUS_WET_LEVEL"));
}

IntuitionAudioProcessor::~IntuitionAudioProcessor() {}

void IntuitionAudioProcessor::resetSynths() {
    synth.clearVoices();
    for (int i = 0; i < 8; ++i) {
        synth.addVoice(
            new UnisonVoice(
                parameters,
                modMatrix,
                &bank1,
                &bank2,
                &bank3,
                &bank4
            )
        );
    }
}

//==============================================================================
const juce::String IntuitionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool IntuitionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool IntuitionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool IntuitionAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double IntuitionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int IntuitionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int IntuitionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void IntuitionAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String IntuitionAudioProcessor::getProgramName (int index)
{
    return {};
}

void IntuitionAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void IntuitionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock) {
    synth.setCurrentPlaybackSampleRate(sampleRate);
    //setLatencySamples(1024);  // For FFT
    fftL.prepare(sampleRate, 0);
    fftR.prepare(sampleRate, 1);

    resetSynths();
    synth.clearSounds();
    synth.addSound(new SineWaveSound());

    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto* v = dynamic_cast<UnisonVoice*>(synth.getVoice(i))) {
            v->prepareToPlay(sampleRate, samplesPerBlock, getNumOutputChannels());
        }
    }

    envManager.prepare(sampleRate);

    delayModule.prepare(getSampleRate(), 2000, getNumOutputChannels());
}

void IntuitionAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool IntuitionAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void IntuitionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    juce::MidiBuffer processMidi;
    for (const auto metadata : midiMessages) {
        auto msg = metadata.getMessage();
        if (msg.isNoteOn()) {
            envManager.noteOn(msg.getNoteNumber());
            //DBG("Note pressed: " << msg.getNoteNumber());
        }
        else if (msg.isNoteOff()) {
            envManager.noteOff(msg.getNoteNumber());
            //DBG("Note released: " << msg.getNoteNumber());
        }

        processMidi.addEvent(msg, metadata.samplePosition);
    }

    //========== LFO Modulation ==========//
    setCurrentBPM();
    float sampleRate = getSampleRate();
    
    calculateLFOFrequency("LFO1_MODE", "LFO1_RATE", "LFO1_SYNC_DIV", lfoRate1);
    float phaseInc1 = lfoRate1 / sampleRate;
    calculateLFOPhase(lfoShape1,lfoPhase1, "LFO1_MODE", "LFO1_SYNC_DIV", "LFO1_RATE", lfoValue1, sampleRate, buffer.getNumSamples());

    calculateLFOFrequency("LFO1_MODE", "LFO1_RATE", "LFO1_SYNC_DIV", lfoRate2);
    float phaseInc2 = lfoRate2 / sampleRate;
    calculateLFOPhase(lfoShape2, lfoPhase2, "LFO2_MODE", "LFO2_SYNC_DIV", "LFO2_RATE", lfoValue2, sampleRate, buffer.getNumSamples());

    calculateLFOFrequency("LFO3_MODE", "LFO3_RATE", "LFO3_SYNC_DIV", lfoRate3);
    float phaseInc3 = lfoRate3 / sampleRate;
    calculateLFOPhase(lfoShape3, lfoPhase3, "LFO3_MODE", "LFO3_SYNC_DIV", "LFO3_RATE", lfoValue3, sampleRate, buffer.getNumSamples());

    //======= Envelope Modulation ========//
    modMatrix.applyMods();

    juce::ADSR::Parameters adsrParams;
    adsrParams.attack = *parameters.getRawParameterValue("ENV_OSC_ATTACK");
    adsrParams.decay = *parameters.getRawParameterValue("ENV_OSC_DECAY");
    adsrParams.sustain = *parameters.getRawParameterValue("ENV_OSC_SUSTAIN");
    adsrParams.release = *parameters.getRawParameterValue("ENV_OSC_RELEASE");

    envManager.setParameters();
    envManager.process(buffer.getNumSamples());

    envValueOsc = envManager.getEnvValue(0);
    envValue1 = envManager.getEnvValue(1);
    envValue2 = envManager.getEnvValue(2);
    envValue3 = envManager.getEnvValue(3);

    int unisonA = static_cast<int>(*parameters.getRawParameterValue("A_UNISON"));
    int unisonB = static_cast<int>(*parameters.getRawParameterValue("B_UNISON"));
    int unisonC = static_cast<int>(*parameters.getRawParameterValue("C_UNISON"));
    int unisonD = static_cast<int>(*parameters.getRawParameterValue("D_UNISON"));

    float detuneA = modMatrix.getModdedDest("A_DETUNE");
    float detuneB = modMatrix.getModdedDest("B_DETUNE");
    float detuneC = modMatrix.getModdedDest("C_DETUNE");
    float detuneD = modMatrix.getModdedDest("D_DETUNE");

    float morphA = modMatrix.getModdedDest("A_MORPH");
    float morphB = modMatrix.getModdedDest("B_MORPH");
    float morphC = modMatrix.getModdedDest("C_MORPH");
    float morphD = modMatrix.getModdedDest("D_MORPH");

    float filterCutoff = modMatrix.getModdedDest("FILTER_CUTOFF");
    float filterResonance = modMatrix.getModdedDest("FILTER_RESONANCE");
    int filterType = (int)*parameters.getRawParameterValue("FILTER_TYPE");

    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto* v = dynamic_cast<UnisonVoice*>(synth.getVoice(i))) {
            v->setEnvelopeParams(adsrParams);

            v->getOscA().setUnison(unisonA);
            v->getOscA().setDetuneRange(detuneA);
            v->getOscA().setMorph(morphA);

            v->getOscB().setUnison(unisonB);
            v->getOscB().setDetuneRange(detuneB);
            v->getOscB().setMorph(morphB);

            v->getOscC().setUnison(unisonC);
            v->getOscC().setDetuneRange(detuneC);
            v->getOscC().setMorph(morphC);

            v->getOscD().setUnison(unisonD);
            v->getOscD().setDetuneRange(detuneD);
            v->getOscD().setMorph(morphD);

            //===== Filter =====//
            v->setFilterCutoff(filterCutoff);
            v->setFilterResonance(filterResonance);
            v->setFilterType(filterType);
        }
    }

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    //============= FFT TESTING ============//
    fftL.processBlock(buffer);
    fftR.processBlock(buffer);

    //======================================//
    
    if (*parameters.getRawParameterValue("REVERB_TOGGLE")) {
        reverbModule.prepare(getSampleRate(), buffer.getNumSamples(), buffer.getNumChannels());
        reverbModule.updateParameters();
        reverbModule.processBlock(buffer);
    }
    if (*parameters.getRawParameterValue("DELAY_TOGGLE")) {
        delayModule.updateParameters();
        delayModule.processBlock(buffer);
    }
    if (*parameters.getRawParameterValue("CHORUS_TOGGLE")) {
        chorusModule.prepare(getSampleRate(), buffer.getNumSamples());
        chorusModule.updateParameters();
        chorusModule.processBlock(buffer);
    }

    //========== FFT TESTING ===============//
    
    float masterVol = *parameters.getRawParameterValue("MASTER");
    buffer.applyGain(masterVol);
}

//==============================================================================
bool IntuitionAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* IntuitionAudioProcessor::createEditor()
{
    return new IntuitionAudioProcessorEditor (*this);
}

//==============================================================================
void IntuitionAudioProcessor::getStateInformation (juce::MemoryBlock& destData) {
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    destData.reset();
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());

    auto removeOldChild = [&](const juce::String tag) {
        auto* oldChild = xml->getChildByName(tag);
        if (oldChild) {
            xml->removeChildElement(oldChild, true);
        }
    };
    removeOldChild("ModConnections");
    removeOldChild("Bank1");
    removeOldChild("Bank2");
    removeOldChild("Bank3");
    removeOldChild("Bank4");

    // Mod connections
    auto* modList = xml->createNewChildElement("ModConnections");
    std::vector<ModConnection*> allMods;
    modMatrix.getAllConnections(allMods);
    for (const auto& conn : allMods) {
        auto* source = conn->getSource();
        auto* dest = conn->getDestination();
        
        auto* mod = modList->createNewChildElement("ModConnection");
        mod->setAttribute("source", source->getName());
        mod->setAttribute("destination", dest->getName());
        mod->setAttribute("active", conn->getActive());
        mod->setAttribute("bipolar", conn->getBipolar());
        mod->setAttribute("opacity", conn->getOpacity());
        mod->setAttribute("depth", conn->getDepth());
    }

    // Wavebank states
    auto* bank1Xml = xml->createNewChildElement("Bank1");
    auto* bank2Xml = xml->createNewChildElement("Bank2");
    auto* bank3Xml = xml->createNewChildElement("Bank3");
    auto* bank4Xml = xml->createNewChildElement("Bank4");

    auto generateBankXml = [&](WavetableBank& bank, juce::XmlElement* element) {
        for (int i = 0; i < bank.size(); ++i) {
            auto& info = bank.getWavetableInfo(i);
            auto* wave = element->createNewChildElement("Wave" + juce::String(i));
            wave->setAttribute("isNative", info.isNative);
            wave->setAttribute("filePath", info.filePath.getFullPathName());
            wave->setAttribute("nativeId", info.nativeId);
        }
    };
    generateBankXml(bank1, bank1Xml);
    generateBankXml(bank2, bank2Xml);
    generateBankXml(bank3, bank3Xml);
    generateBankXml(bank4, bank4Xml);

    copyXmlToBinary(*xml, destData);
}

void IntuitionAudioProcessor::setStateInformation (const void* data, int sizeInBytes) {
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    DBG(xmlState->toString());

    if (!xmlState.get()) return;
    if (xmlState->hasTagName(parameters.state.getType())) {
        parameters.replaceState(juce::ValueTree::fromXml(*xmlState));

        // Rebuilding modConnections
        if (auto* modList = xmlState->getChildByName("ModConnections")) {
            forEachXmlChildElement(*modList, modElement) {
                //DBG("ModConnection child reached");
                if (modElement->hasTagName("ModConnection")) {
                    juce::String sourceId = modElement->getStringAttribute("source");
                    juce::String destId = modElement->getStringAttribute("destination");
                    modMatrix.addConnection(sourceId, destId);

                    auto* conn = modMatrix.getConnection(sourceId, destId);
                    conn->setActive(modElement->getBoolAttribute("active"));
                    conn->setBipolar(modElement->getBoolAttribute("bipolar"));
                    conn->setOpacity(modElement->getDoubleAttribute("opacity"));
                    conn->setDepth(modElement->getDoubleAttribute("depth"));
                }
            }
        }

        // Rebuilding wavebanks
        auto refillBank = [&](WavetableBank& bank, const juce::String tag) {
            bank.clear();
            if (auto* bankXml = xmlState->getChildByName(tag)) {
                forEachXmlChildElement(*bankXml, modElement) {
                    if (modElement->getBoolAttribute("isNative")) {
                        auto binary = WavetableHelper::getWavBinary(modElement->getIntAttribute("nativeId"));
                        bank.addWavetable(binary.first, binary.second);
                    }
                    else {  // Load from file
                        juce::File wavFile(modElement->getStringAttribute("filePath"));
                        bank.addWavetable(wavFile);
                    }
                }
            }
        };
        refillBank(bank1, "Bank1");
        refillBank(bank2, "Bank2");
        refillBank(bank3, "Bank3");
        refillBank(bank4, "Bank4");
    }

    const char* wav = BinaryData::AKWF_sin_wav;
    int wavSize = BinaryData::AKWF_sin_wavSize;
    if (bank1.size() == 0) {
        bank1.addWavetable(wav, wavSize);
    }
    if (bank2.size() == 0) {
        bank2.addWavetable(wav, wavSize);
    }
    if (bank3.size() == 0) {
        bank3.addWavetable(wav, wavSize);
    }
    if (bank4.size() == 0) {
        bank4.addWavetable(wav, wavSize);
    }
}

juce::String IntuitionAudioProcessor::getParametersAsJsonString() {
    juce::DynamicObject::Ptr root = new juce::DynamicObject();

    auto setFloat = [&](const juce::String& id) {  // Thanks, Ahmed!
        if (auto* p = parameters.getRawParameterValue(id)) {
            root->setProperty(id, juce::var(static_cast<double>(p->load())));
        }
    };
    auto setInt = [&](const juce::String& id) {
        if (auto* p = parameters.getRawParameterValue(id)) {
            root->setProperty(id, juce::var(static_cast<int>(std::lround(p->load()))));
        }
    };
    auto setBool = [&](const juce::String& id) {
        if (auto* p = parameters.getRawParameterValue(id)) {
            root->setProperty(id, juce::var(p->load() >= 0.5f));
        }
    };

    setFloat("MASTER");

    // ===== Envelopes (OSC env + 3 user envs)
    setFloat("ENV_OSC_ATTACK");
    setFloat("ENV_OSC_DECAY");
    setFloat("ENV_OSC_SUSTAIN");
    setFloat("ENV_OSC_RELEASE");

    setFloat("ENV1_ATTACK");
    setFloat("ENV1_DECAY");
    setFloat("ENV1_SUSTAIN");
    setFloat("ENV1_RELEASE");

    setFloat("ENV2_ATTACK");
    setFloat("ENV2_DECAY");
    setFloat("ENV2_SUSTAIN");
    setFloat("ENV2_RELEASE");

    setFloat("ENV3_ATTACK");
    setFloat("ENV3_DECAY");
    setFloat("ENV3_SUSTAIN");
    setFloat("ENV3_RELEASE");

    // ===== Oscillator A =====
    setBool("A_TOGGLE");
    setFloat("A_VOLUME");
    setInt("A_UNISON");
    setFloat("A_DETUNE");
    setFloat("A_MORPH");
    setInt("A_OCTAVE");
    setInt("A_COARSE");
    setInt("A_FINE");

    // ===== Oscillator B =====
    setBool("B_TOGGLE");
    setFloat("B_VOLUME");
    setInt("B_UNISON");
    setFloat("B_DETUNE");
    setFloat("B_MORPH");
    setInt("B_OCTAVE");
    setInt("B_COARSE");
    setInt("B_FINE");

    // ===== Oscillator C =====
    setBool("C_TOGGLE");
    setFloat("C_VOLUME");
    setInt("C_UNISON");
    setFloat("C_DETUNE");
    setFloat("C_MORPH");
    setInt("C_OCTAVE");
    setInt("C_COARSE");
    setInt("C_FINE");

    // ===== Oscillator D =====
    setBool("D_TOGGLE");
    setFloat("D_VOLUME");
    setInt("D_UNISON");
    setFloat("D_DETUNE");
    setFloat("D_MORPH");
    setInt("D_OCTAVE");
    setInt("D_COARSE");
    setInt("D_FINE");

    // ===== Filter Sends =====
    setBool("A_FILTER_SEND");
    setBool("B_FILTER_SEND");
    setBool("C_FILTER_SEND");
    setBool("D_FILTER_SEND");

    // ===== LFOs (mode & sync are choice) =====
    // LFO1
    setInt("LFO1_MODE");
    setInt("LFO1_SYNC_DIV");
    setFloat("LFO1_RATE");
    // LFO2
    setInt("LFO2_MODE");
    setInt("LFO2_SYNC_DIV");
    setFloat("LFO2_RATE");
    // LFO3
    setInt("LFO3_MODE");
    setInt("LFO3_SYNC_DIV");
    setFloat("LFO3_RATE");

    // ===== Filter =====
    setFloat("FILTER_CUTOFF");
    setFloat("FILTER_RESONANCE");
    setInt("FILTER_TYPE");

    // ===== Reverb =====
    setBool("REVERB_TOGGLE");
    setFloat("REVERB_DAMPING");
    setFloat("REVERB_ROOM_SIZE");
    setFloat("REVERB_WIDTH");
    setFloat("REVERB_DRY_LEVEL");
    setFloat("REVERB_WET_LEVEL");

    juce::var jsonVar(root.get());
    return juce::JSON::toString(jsonVar, true);
}

void IntuitionAudioProcessor::applyJsonParameterTweaks(juce::var& jsonTweaks) {
    juce::DynamicObject::Ptr obj = jsonTweaks.getDynamicObject();
    jassert(obj);

    for (auto& prop : obj->getProperties()) {
        parameters.getParameterAsValue(prop.name).setValue(obj->getProperty(prop.name));

        DBG(prop.name.toString() << ": " << obj->getProperty(prop.name).toString());
    }
}

void IntuitionAudioProcessor::initializeUserDirectory() {
    juce::File documents = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory);
    juce::File home = documents.getChildFile("Intuition");

    if (!home.exists()) home.createDirectory();

    // Organizing subfolders
    auto wavetables = home.getChildFile("Waveforms");  // Users can organize this folder however they please
    auto presets = home.getChildFile("Presets");
    auto skins = home.getChildFile("Skins");
    auto logs = home.getChildFile("Logs");

    wavetables.createDirectory();
    presets.createDirectory();
    skins.createDirectory();
    logs.createDirectory();
}

float IntuitionAudioProcessor::getDivisionFloat(int syncDiv) {
    float div = 1.0f;
    
    if (syncDiv <= 7) {
        div = 4.0f * pow(0.5f, syncDiv);
    }

    return div;
}

void IntuitionAudioProcessor::calculateLFOFrequency(
    const juce::String modeName,
    const juce::String rateName,
    const juce::String syncDivName,
    float& rateVal
) {
    int mode = (int)*parameters.getRawParameterValue(modeName);

    if (mode == 0) {
        rateVal = *parameters.getRawParameterValue(rateName);
    }
    else if (mode == 1) {
        float division = getDivisionFloat((int)*parameters.getRawParameterValue(syncDivName));
        rateVal = (currentBPM / 60.0f) * division;
    }
}

void IntuitionAudioProcessor::calculateLFOPhase(
    LFOShape& shape,
    float& phase,
    const juce::String modeName,
    const juce::String syncDivName,
    const juce::String rateName,
    float& lfoValue,
    float sampleRate,
    int numSamples
) {
    int mode = (int)*parameters.getRawParameterValue(modeName);
    
    if (mode == 1) {  // BPM Sync
        // Getting tempo multiplier
        int tempoIndex = static_cast<int>(*parameters.getRawParameterValue(syncDivName));
        TempoDivision division = static_cast<TempoDivision>(tempoIndex);
        float beatsPerCycle = divisionToBeats(division);

        if (auto* playHead = getPlayHead()) {
            if (auto position = playHead->getPosition()) {
                bool isPlaying = position->getIsPlaying();
                auto bpmOpt = position->getBpm();
                auto ppqOpt = position->getPpqPosition();

                if (isPlaying && ppqOpt) {  // DAW is actively playing/in playback
                    double ppq = *ppqOpt;
                    phase = static_cast<float>(fmod(ppq / beatsPerCycle, 1.0));
                    lfoValue = shape.getValue(phase);
                    return;
                }
                else {  // DAW is idle/no trace
                    double bpm = bpmOpt && (*bpmOpt > 0.0) ? *bpmOpt : 120.0;
                    float phaseInc = bpm / (60.0f * beatsPerCycle * sampleRate);
                    for (int sample = 0; sample < numSamples; ++sample) {
                        phase += phaseInc;
                        while (phase > 1.0f) {
                            phase -= 1.0f;
                        }
                        lfoValue = shape.getValue(phase);
                    }
                    return;
                }
            }
        }
        DBG("Error retrieving PlayHead info");
    }
    else if (mode == 0) {  // Free Run (No tempo sync)
        float rate = *parameters.getRawParameterValue(rateName);
        float phaseInc = rate / sampleRate;
        for (int sample = 0; sample < numSamples; ++sample) {
            phase += phaseInc;
            while (phase > 1.0f) {
                phase -= 1.0f;
            }

            lfoValue = shape.getValue(phase);
        }
    }
}

void IntuitionAudioProcessor::setCurrentBPM() {
    juce::AudioPlayHead::CurrentPositionInfo posInfo;
    if (auto* playHead = getPlayHead()) {
        playHead->getCurrentPosition(posInfo);
    }
    currentBPM = posInfo.bpm > 0.0 ? posInfo.bpm : 120.0f;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new IntuitionAudioProcessor();
}
