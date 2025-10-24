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
        std::make_unique<juce::AudioParameterFloat>("MASTER", "Master Volume", 0.0f, 1.0f, 0.75f),

        std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.0f, 1.0f, 0.0f),

        //=============== Oscillators ================//
        std::make_unique<juce::AudioParameterBool>("A_TOGGLE", "A Toggle", true),
        std::make_unique<juce::AudioParameterInt>("A_UNISON", "A Unison", 1, 8, 1),
        std::make_unique<juce::AudioParameterFloat>("A_DETUNE", "A Detune", 0, 100, 0),
        std::make_unique<juce::AudioParameterFloat>("A_MORPH", "A Morph", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterInt>("A_OCTAVE", "A Octave", -4, 4, 0),
        std::make_unique<juce::AudioParameterInt>("A_COARSE", "A Coarse Pitch", -12, 12, 0),
        std::make_unique<juce::AudioParameterInt>("A_FINE", "A Fine Pitch", -100, 100, 0),

        std::make_unique<juce::AudioParameterBool>("B_TOGGLE", "B Toggle", false),
        std::make_unique<juce::AudioParameterInt>("B_UNISON", "B Unison", 1, 8, 1),
        std::make_unique<juce::AudioParameterFloat>("B_DETUNE", "B Detune", 0, 100, 0),
        std::make_unique<juce::AudioParameterFloat>("B_MORPH", "B Morph", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterInt>("B_OCTAVE", "B Octave", -4, 4, 0),
        std::make_unique<juce::AudioParameterInt>("B_COARSE", "B Coarse Pitch", -12, 12, 0),
        std::make_unique<juce::AudioParameterInt>("B_FINE", "B Fine Pitch", -100, 100, 0),

        std::make_unique<juce::AudioParameterBool>("C_TOGGLE", "C Toggle", false),
        std::make_unique<juce::AudioParameterInt>("C_UNISON", "C Unison", 1, 8, 1),
        std::make_unique<juce::AudioParameterFloat>("C_DETUNE", "C Detune", 0, 100, 0),
        std::make_unique<juce::AudioParameterFloat>("C_MORPH", "C Morph", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterInt>("C_OCTAVE", "C Octave", -4, 4, 0),
        std::make_unique<juce::AudioParameterInt>("C_COARSE", "C Coarse Pitch", -12, 12, 0),
        std::make_unique<juce::AudioParameterInt>("C_FINE", "C Fine Pitch", -100, 100, 0),

        std::make_unique<juce::AudioParameterBool>("D_TOGGLE", "D Toggle", false),
        std::make_unique<juce::AudioParameterInt>("D_UNISON", "D Unison", 1, 8, 1),
        std::make_unique<juce::AudioParameterFloat>("D_DETUNE", "D Detune", 0, 100, 0),
        std::make_unique<juce::AudioParameterFloat>("D_MORPH", "D Morph", 0.0f, 1.0f, 0.0f),
        std::make_unique<juce::AudioParameterInt>("D_OCTAVE", "D Octave", -4, 4, 0),
        std::make_unique<juce::AudioParameterInt>("D_COARSE", "D Coarse Pitch", -12, 12, 0),
        std::make_unique<juce::AudioParameterInt>("D_FINE", "D Fine Pitch", -100, 100, 0),

        //=============== LFOs ===============//
        std::make_unique<juce::AudioParameterChoice>("LFO1_MODE", "LFO 1 Mode", juce::StringArray{"Free", "Synced"}, 0),
        std::make_unique<juce::AudioParameterChoice>("LFO1_SYNC_DIV", "LFO 1 Sync Division", juce::StringArray{"1/1", "1/2", "1/4", "1/8", "1/16", "1/32"}, 2),
        std::make_unique<juce::AudioParameterFloat>("LFO1_RATE", "LFO 1 Rate", 0.01f, 30.0f, 1.0f),

        std::make_unique<juce::AudioParameterChoice>("LFO2_MODE", "LFO 2 Mode", juce::StringArray{"Free", "Synced"}, 0),
        std::make_unique<juce::AudioParameterChoice>("LFO2_SYNC_DIV", "LFO 2 Sync Division", juce::StringArray{"1/1", "1/2", "1/4", "1/8", "1/16", "1/32"}, 2),
        std::make_unique<juce::AudioParameterFloat>("LFO2_RATE", "LFO 2 Rate", 0.01f, 30.0f, 1.0f),

        std::make_unique<juce::AudioParameterChoice>("LFO3_MODE", "LFO 3 Mode", juce::StringArray{"Free", "Synced"}, 0),
        std::make_unique<juce::AudioParameterChoice>("LFO3_SYNC_DIV", "LFO 3 Sync Division", juce::StringArray{"1/1", "1/2", "1/4", "1/8", "1/16", "1/32"}, 2),
        std::make_unique<juce::AudioParameterFloat>("LFO3_RATE", "LFO 3 Rate", 0.01f, 30.0f, 1.0f),

        //============== Filter ==============//
        std::make_unique<juce::AudioParameterBool>("A_FILTER_SEND", "A Filter Send", false),
        std::make_unique<juce::AudioParameterBool>("B_FILTER_SEND", "A Filter Send", false),
        std::make_unique<juce::AudioParameterBool>("C_FILTER_SEND", "A Filter Send", false),
        std::make_unique<juce::AudioParameterBool>("D_FILTER_SEND", "A Filter Send", false),

        std::make_unique<juce::AudioParameterFloat>("FILTER_CUTOFF", "Filter Cutoff Frequency", 20.0f, 20000.0f, 1000.0f),
        std::make_unique<juce::AudioParameterFloat>("FILTER_RESONANCE", "Filter Cutoff Frequency", 0.01f, 1.0f, 0.7f),
        std::make_unique<juce::AudioParameterChoice>("FILTER_TYPE", "Filter Type", juce::StringArray{"Low", "High", "Band"}, 0),
    }),
    context(
        this,
        parameters,
        &modMatrix,

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
    )
{
    parameters.state = juce::ValueTree("PARAMETERS");

    /*juce::File file("C:/Users/BroDe/Downloads/AKWF/AKWF_cello/AKWF_cello_0001.wav");
    bank1.addWavetable(file);
    bank2.addWavetable(file);
    bank3.addWavetable(file);
    bank4.addWavetable(file);*/

    const void* wav = BinaryData::AKWF_saw_wav;
    int wavSize = BinaryData::AKWF_saw_wavSize;
    bank1.addWavetable(wav, wavSize);
    bank2.addWavetable(wav, wavSize);
    bank3.addWavetable(wav, wavSize);
    bank4.addWavetable(wav, wavSize);

    resetSynths();

    //========== ModMatrix Setup ==========//
    //===== Sources
    ModSource* lfoSource1 = modMatrix.addSource("LFO1");
    lfoSource1->setValuePtr(&lfoValue1);

    //===== Destinations
    ModDestination* aOctDest = modMatrix.addDestination("A_OCTAVE");
    ModDestination* aCoarseDest = modMatrix.addDestination("A_COARSE");
    ModDestination* aFineDest = modMatrix.addDestination("A_FINE");
    ModDestination* aMorphDest = modMatrix.addDestination("A_MORPH");
    ModDestination* aDetuneDest = modMatrix.addDestination("A_DETUNE");
    
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

    ModDestination* bOctDest = modMatrix.addDestination("B_OCTAVE");
    ModDestination* bCoarseDest = modMatrix.addDestination("B_COARSE");
    ModDestination* bFineDest = modMatrix.addDestination("B_FINE");
    ModDestination* bMorphDest = modMatrix.addDestination("B_MORPH");
    ModDestination* bDetuneDest = modMatrix.addDestination("B_DETUNE");

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

    ModDestination* cOctDest = modMatrix.addDestination("C_OCTAVE");
    ModDestination* cCoarseDest = modMatrix.addDestination("C_COARSE");
    ModDestination* cFineDest = modMatrix.addDestination("C_FINE");
    ModDestination* cMorphDest = modMatrix.addDestination("C_MORPH");
    ModDestination* cDetuneDest = modMatrix.addDestination("C_DETUNE");

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

    ModDestination* dOctDest = modMatrix.addDestination("D_OCTAVE");
    ModDestination* dCoarseDest = modMatrix.addDestination("D_COARSE");
    ModDestination* dFineDest = modMatrix.addDestination("D_FINE");
    ModDestination* dMorphDest = modMatrix.addDestination("D_MORPH");
    ModDestination* dDetuneDest = modMatrix.addDestination("D_DETUNE");

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

    ModDestination* filterCutoffDest = modMatrix.addDestination("FILTER_CUTOFF");
    ModDestination* filterResonanceDest = modMatrix.addDestination("FILTER_RESONANCE");

    filterCutoffDest->setBasePtr(parameters.getRawParameterValue("FILTER_CUTOFF"));
    filterCutoffDest->setMinRange(20.0f);
    filterCutoffDest->setMaxRange(20000.0f);
    filterResonanceDest->setBasePtr(parameters.getRawParameterValue("FILTER_RESONANCE"));
    filterResonanceDest->setMinRange(0.01f);
    filterResonanceDest->setMaxRange(1.0f);
}

IntuitionAudioProcessor::~IntuitionAudioProcessor() {}

float* IntuitionAudioProcessor::getLFOPhase(int lfoNum)
{
    switch (lfoNum) {
    case 1:
        return &lfoPhase1;
    case 2:
        return &lfoPhase2;
    case 3:
        return &lfoPhase3;
    default:
        return nullptr;
    }
}

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
void IntuitionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate(sampleRate);

    resetSynths();
    synth.clearSounds();
    synth.addSound(new SineWaveSound());

    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto* v = dynamic_cast<UnisonVoice*>(synth.getVoice(i))) {
            v->prepareToPlay(sampleRate, samplesPerBlock, getNumOutputChannels());
        }
    }
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

void IntuitionAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    //========== LFO Modulation ==========//
    setCurrentBPM();
    float sampleRate = getSampleRate();
    
    calculateLFOFrequency("LFO1_MODE", "LFO1_RATE", "LFO1_SYNC_DIV", lfoRate1);
    float phaseInc1 = lfoRate1 / sampleRate;
    calculateLFOPhase(
        lfoShape1,
        lfoPhase1,
        "LFO1_MODE",
        "LFO1_SYNC_DIV",
        "LFO1_RATE",
        lfoValue1,
        sampleRate,
        buffer.getNumSamples()
    );

    modMatrix.applyMods();

    juce::ADSR::Parameters adsrParams;
    adsrParams.attack = *parameters.getRawParameterValue("ATTACK");
    adsrParams.decay = *parameters.getRawParameterValue("DECAY");
    adsrParams.sustain = *parameters.getRawParameterValue("SUSTAIN");
    adsrParams.release = *parameters.getRawParameterValue("RELEASE");

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
void IntuitionAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void IntuitionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

float IntuitionAudioProcessor::getDivisionFloat(int syncDiv) {
    float div = 1.0f;
    switch (syncDiv) {
    case 0:  // 1/1
        div = 1.0f;
        break;
    case 1:  // 1/2
        div = 0.5f;
        break;
    case 2:  // 1/4
        div = 0.25f;
        break;
    case 3:  // 1/8
        div = 0.125f;
        break;
    case 4:  // 1/16
        div = 0.0625f;
        break;
    case 5:  // 1/32
        div = 0.03125f;
        break;
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
        juce::AudioPlayHead::CurrentPositionInfo posInfo;
        auto* playHead = getPlayHead();
        if (playHead && playHead->getCurrentPosition(posInfo) && posInfo.isPlaying) {
            double ppq = posInfo.ppqPosition;
            float beatsPerCycle = getDivisionFloat((int)*parameters.getRawParameterValue(syncDivName));
            phase = static_cast<float>(fmod(ppq / beatsPerCycle, 1.0));
            lfoValue = shape.getValue(phase);
            return;
        }
        else if (playHead && playHead->getCurrentPosition(posInfo) && !posInfo.isPlaying) {
            double bpm = posInfo.bpm > 0.0 ? posInfo.bpm : 120.0;
            float beatsPerCycle = getDivisionFloat((int)*parameters.getRawParameterValue(syncDivName));
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
        DBG("Error retrieving PlayHead info");
    }
    else if (mode == 0) {  // Free Run
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
