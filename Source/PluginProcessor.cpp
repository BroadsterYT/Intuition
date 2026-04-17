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

        //============= Equalizer ============//
        std::make_unique<juce::AudioParameterChoice>("EQBAND1_FILTER_TYPE", "EQ Band 1 Filter Type", juce::StringArray{ "HighPass", "HighShelf", "Peaking", "LowShelf", "LowPass", "Notch"}, 2),
        std::make_unique<juce::AudioParameterChoice>("EQBAND2_FILTER_TYPE", "EQ Band 2 Filter Type", juce::StringArray{ "HighPass", "HighShelf", "Peaking", "LowShelf", "LowPass", "Notch" }, 2),
        std::make_unique<juce::AudioParameterChoice>("EQBAND3_FILTER_TYPE", "EQ Band 3 Filter Type", juce::StringArray{ "HighPass", "HighShelf", "Peaking", "LowShelf", "LowPass", "Notch" }, 2),
        std::make_unique<juce::AudioParameterChoice>("EQBAND4_FILTER_TYPE", "EQ Band 4 Filter Type", juce::StringArray{ "HighPass", "HighShelf", "Peaking", "LowShelf", "LowPass", "Notch" }, 2),
        std::make_unique<juce::AudioParameterChoice>("EQBAND5_FILTER_TYPE", "EQ Band 5 Filter Type", juce::StringArray{ "HighPass", "HighShelf", "Peaking", "LowShelf", "LowPass", "Notch" }, 2),
        std::make_unique<juce::AudioParameterChoice>("EQBAND6_FILTER_TYPE", "EQ Band 6 Filter Type", juce::StringArray{ "HighPass", "HighShelf", "Peaking", "LowShelf", "LowPass", "Notch" }, 2),
        std::make_unique<juce::AudioParameterChoice>("EQBAND7_FILTER_TYPE", "EQ Band 7 Filter Type", juce::StringArray{ "HighPass", "HighShelf", "Peaking", "LowShelf", "LowPass", "Notch" }, 2),
        std::make_unique<juce::AudioParameterChoice>("EQBAND8_FILTER_TYPE", "EQ Band 8 Filter Type", juce::StringArray{ "HighPass", "HighShelf", "Peaking", "LowShelf", "LowPass", "Notch" }, 2),

        std::make_unique<juce::AudioParameterFloat>("EQBAND1_FREQUENCY", "EQ Band 1 Frequency", 20.0f, 20000.0f, 63.0f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND2_FREQUENCY", "EQ Band 2 Frequency", 20.0f, 20000.0f, 122.0f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND3_FREQUENCY", "EQ Band 3 Frequency", 20.0f, 20000.0f, 235.0f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND4_FREQUENCY", "EQ Band 4 Frequency", 20.0f, 20000.0f, 454.0f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND5_FREQUENCY", "EQ Band 5 Frequency", 20.0f, 20000.0f, 877.0f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND6_FREQUENCY", "EQ Band 6 Frequency", 20.0f, 20000.0f, 1693.0f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND7_FREQUENCY", "EQ Band 7 Frequency", 20.0f, 20000.0f, 3269.0f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND8_FREQUENCY", "EQ Band 8 Frequency", 20.0f, 20000.0f, 6324.0f),
        
        std::make_unique<juce::AudioParameterFloat>("EQBAND1_GAIN", "EQ Band 1 Gain", -18.0f, 18.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND2_GAIN", "EQ Band 2 Gain", -18.0f, 18.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND3_GAIN", "EQ Band 3 Gain", -18.0f, 18.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND4_GAIN", "EQ Band 4 Gain", -18.0f, 18.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND5_GAIN", "EQ Band 5 Gain", -18.0f, 18.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND6_GAIN", "EQ Band 6 Gain", -18.0f, 18.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND7_GAIN", "EQ Band 7 Gain", -18.0f, 18.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND8_GAIN", "EQ Band 8 Gain", -18.0f, 18.0f, 0.0f),
        
        std::make_unique<juce::AudioParameterFloat>("EQBAND1_Q", "EQ Band 1 Quality Factor", 0.2f, 12.0f, 0.707f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND2_Q", "EQ Band 2 Quality Factor", 0.2f, 12.0f, 0.707f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND3_Q", "EQ Band 3 Quality Factor", 0.2f, 12.0f, 0.707f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND4_Q", "EQ Band 4 Quality Factor", 0.2f, 12.0f, 0.707f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND5_Q", "EQ Band 5 Quality Factor", 0.2f, 12.0f, 0.707f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND6_Q", "EQ Band 6 Quality Factor", 0.2f, 12.0f, 0.707f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND7_Q", "EQ Band 7 Quality Factor", 0.2f, 12.0f, 0.707f),
        std::make_unique<juce::AudioParameterFloat>("EQBAND8_Q", "EQ Band 8 Quality Factor", 0.2f, 12.0f, 0.707f),
    }),
    lfo1(parameters, "LFO1_MODE", "LFO1_SYNC_DIV", "LFO1_RATE"),
    lfo2(parameters, "LFO2_MODE", "LFO2_SYNC_DIV", "LFO2_RATE"),
    lfo3(parameters, "LFO3_MODE", "LFO3_SYNC_DIV", "LFO3_RATE"),
    context(
        this,
        parameters,
        &modMatrix,
        envManager,

        bank1,
        bank2,
        bank3,
        bank4,

        lfo1.getShapeRef(),
        lfo2.getShapeRef(),
        lfo3.getShapeRef(),
        lfo1.getPhasePtr(),
        lfo2.getPhasePtr(),
        lfo3.getPhasePtr(),

        equalizerModule
    ),
    envManager(parameters),
    reverbModule(parameters, &modMatrix),
    delayModule(parameters, &modMatrix),
    chorusModule(parameters, &modMatrix),
    equalizerModule(parameters, &modMatrix)
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
    lfoSource1->setValuePtr(lfo1.getValuePtr());
    ModSource* lfoSource2 = modMatrix.addSource("LFO2");
    lfoSource2->setValuePtr(lfo2.getValuePtr());
    ModSource* lfoSource3 = modMatrix.addSource("LFO3");
    lfoSource3->setValuePtr(lfo3.getValuePtr());

    ModSource* envSourceA = modMatrix.addSource("ENV_Osc");
    envSourceA->setValuePtr(&envValueOsc);

    ModSource* envSource1 = modMatrix.addSource("ENV1");
    envSource1->setValuePtr(&envValue1);
    ModSource* envSource2 = modMatrix.addSource("ENV2");
    envSource2->setValuePtr(&envValue2);
    ModSource* envSource3 = modMatrix.addSource("ENV3");
    envSource3->setValuePtr(&envValue3);

    //===== Destinations

    createModDestination("A_OCTAVE", -4, 4);
    createModDestination("A_COARSE", -12, 12);
    createModDestination("A_FINE", -100, 100);
    createModDestination("A_MORPH", 0.0f, 1.0f);
    createModDestination("A_DETUNE", 0.0f, 1.0f);
    createModDestination("A_VOLUME", 0.0f, 1.0f);
    createModDestination("A_PAN", 0.0f, 1.0f);

    createModDestination("B_OCTAVE", -4, 4);
    createModDestination("B_COARSE", -12, 12);
    createModDestination("B_FINE", -100, 100);
    createModDestination("B_MORPH", 0.0f, 1.0f);
    createModDestination("B_DETUNE", 0.0f, 1.0f);
    createModDestination("B_VOLUME", 0.0f, 1.0f);
    createModDestination("B_PAN", 0.0f, 1.0f);

    createModDestination("C_OCTAVE", -4, 4);
    createModDestination("C_COARSE", -12, 12);
    createModDestination("C_FINE", -100, 100);
    createModDestination("C_MORPH", 0.0f, 1.0f);
    createModDestination("C_DETUNE", 0.0f, 1.0f);
    createModDestination("C_VOLUME", 0.0f, 1.0f);
    createModDestination("C_PAN", 0.0f, 1.0f);

    createModDestination("D_OCTAVE", -4, 4);
    createModDestination("D_COARSE", -12, 12);
    createModDestination("D_FINE", -100, 100);
    createModDestination("D_MORPH", 0.0f, 1.0f);
    createModDestination("D_DETUNE", 0.0f, 1.0f);
    createModDestination("D_VOLUME", 0.0f, 1.0f);
    createModDestination("D_PAN", 0.0f, 1.0f);

    createModDestination("FILTER_CUTOFF", 20.0f, 20000.0f);
    createModDestination("FILTER_RESONANCE", 0.01f, 1.0f);

    //=== Reverb
    createModDestination("REVERB_DAMPING");
    createModDestination("REVERB_ROOM_SIZE");
    createModDestination("REVERB_WIDTH");
    createModDestination("REVERB_DRY_LEVEL");
    createModDestination("REVERB_WET_LEVEL");

    //=== Delay
    createModDestination("DELAY_TIME_MS", 0, 2000);
    createModDestination("DELAY_FEEDBACK");
    createModDestination("DELAY_CUTOFF", 20.0f, 20000.0f);
    createModDestination("DELAY_DRY_LEVEL");
    createModDestination("DELAY_WET_LEVEL");

    //=== Chorus
    createModDestination("CHORUS_RATE", 0.05f, 5.0f);
    createModDestination("CHORUS_DEPTH", 0.5f, 8.0f);
    createModDestination("CHORUS_WIDTH");
    createModDestination("CHORUS_DRY_LEVEL");
    createModDestination("CHORUS_WET_LEVEL");

    //=== Equalizer
    createModDestination("EQBAND1_FREQUENCY", 20.0f, 20000.0f);
    createModDestination("EQBAND2_FREQUENCY", 20.0f, 20000.0f);
    createModDestination("EQBAND3_FREQUENCY", 20.0f, 20000.0f);
    createModDestination("EQBAND4_FREQUENCY", 20.0f, 20000.0f);
    createModDestination("EQBAND5_FREQUENCY", 20.0f, 20000.0f);
    createModDestination("EQBAND6_FREQUENCY", 20.0f, 20000.0f);
    createModDestination("EQBAND7_FREQUENCY", 20.0f, 20000.0f);
    createModDestination("EQBAND8_FREQUENCY", 20.0f, 20000.0f);

    createModDestination("EQBAND1_GAIN", -18.0f, 18.0f);
    createModDestination("EQBAND2_GAIN", -18.0f, 18.0f);
    createModDestination("EQBAND3_GAIN", -18.0f, 18.0f);
    createModDestination("EQBAND4_GAIN", -18.0f, 18.0f);
    createModDestination("EQBAND5_GAIN", -18.0f, 18.0f);
    createModDestination("EQBAND6_GAIN", -18.0f, 18.0f);
    createModDestination("EQBAND7_GAIN", -18.0f, 18.0f);
    createModDestination("EQBAND8_GAIN", -18.0f, 18.0f);

    createModDestination("EQBAND1_Q", 0.2f, 12.0f);
    createModDestination("EQBAND2_Q", 0.2f, 12.0f);
    createModDestination("EQBAND3_Q", 0.2f, 12.0f);
    createModDestination("EQBAND4_Q", 0.2f, 12.0f);
    createModDestination("EQBAND5_Q", 0.2f, 12.0f);
    createModDestination("EQBAND6_Q", 0.2f, 12.0f);
    createModDestination("EQBAND7_Q", 0.2f, 12.0f);
    createModDestination("EQBAND8_Q", 0.2f, 12.0f);
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

    lfo1.prepare(sampleRate);
    lfo2.prepare(sampleRate);
    lfo3.prepare(sampleRate);

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

    lfo1.updateLFOFrequency(currentBPM);
    lfo2.updateLFOFrequency(currentBPM);
    lfo3.updateLFOFrequency(currentBPM);

    auto updateAllLFOPhases = [&](double ppq) {
        int numSamples = buffer.getNumSamples();
        lfo1.updateLFOPhase(currentBPM, ppq, numSamples);
        lfo2.updateLFOPhase(currentBPM, ppq, numSamples);
        lfo3.updateLFOPhase(currentBPM, ppq, numSamples);
    };
    
    auto* playHead = getPlayHead();
    if (playHead) {
        auto position = playHead->getPosition();
        bool isPlaying = position->getIsPlaying();
        auto bpmOpt = position->getBpm();
        auto ppqOpt = position->getPpqPosition();

        if (isPlaying && ppqOpt) {
            double ppq = *ppqOpt;
            updateAllLFOPhases(ppq);
        }
        else {
            updateAllLFOPhases(0.0);
        }
    }
    else {
        updateAllLFOPhases(0.0);
    }

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
    equalizerModule.prepare(getSampleRate(), buffer.getNumSamples(), buffer.getNumChannels());
    equalizerModule.updateParameters();
    equalizerModule.processBlock(buffer);

    
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

ModDestination* IntuitionAudioProcessor::createModDestination(const juce::String& destName, float minRange, float maxRange) {
    ModDestination* dest = modMatrix.addDestination(destName);
    dest->setBasePtr(parameters.getRawParameterValue(destName));
    dest->setMinRange(minRange);
    dest->setMaxRange(maxRange);
    return dest;
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
