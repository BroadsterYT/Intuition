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
        std::make_unique<juce::AudioParameterFloat>("LFO1_RATE", "LFO 1 Rate", 0.0f, 30.0f, 1.0f),
    })
{
    parameters.state = juce::ValueTree("PARAMETERS");

    juce::File file("C:/Users/BroDe/Downloads/AKWF/AKWF_cello/AKWF_cello_0001.wav");
    bank1.addWavetable(file);
    bank2.addWavetable(file);
    bank3.addWavetable(file);
    bank4.addWavetable(file);

    resetSynths();

    //========== ModMatrix Setup ==========//
    //===== Sources
    ModSource* lfoSource1 = modMatrix.addSource("LFO1");

    //===== Destinations
    ModDestination* aOctDest = modMatrix.addDestination("A_OCTAVE");
    ModDestination* aCoarseDest = modMatrix.addDestination("A_COARSE");
    ModDestination* aFineDest = modMatrix.addDestination("A_FINE");
    aOctDest->setBasePtr(parameters.getRawParameterValue("A_OCTAVE"));
    aCoarseDest->setBasePtr(parameters.getRawParameterValue("A_COARSE"));
    aFineDest->setBasePtr(parameters.getRawParameterValue("A_FINE"));

    /*ModDestination* bOctDest = modMatrix.addDestination("B_OCTAVE");
    ModDestination* bCourseDest = modMatrix.addDestination("B_COARSE");
    ModDestination* bFineDest = modMatrix.addDestination("B_FINE");

    ModDestination* cOctDest = modMatrix.addDestination("C_OCTAVE");
    ModDestination* cCourseDest = modMatrix.addDestination("C_COARSE");
    ModDestination* cFineDest = modMatrix.addDestination("C_FINE");

    ModDestination* dOctDest = modMatrix.addDestination("D_OCTAVE");
    ModDestination* dCourseDest = modMatrix.addDestination("D_COARSE");
    ModDestination* dFineDest = modMatrix.addDestination("D_FINE");*/

    /*modMatrix.addConnection("LFO1", "A_COARSE");
    
    ModConnection* test = modMatrix.getConnection("LFO1", "A_COARSE");
    test->getDestination()->setMinRange(-12.0f);
    test->getDestination()->setMaxRange(12.0f);

    test->getSource()->setValuePtr(&lfoValue1);
    test->getDestination()->setBasePtr(parameters.getRawParameterValue("A_COARSE"));*/
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
void IntuitionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synth.setCurrentPlaybackSampleRate(sampleRate);

    resetSynths();
    synth.clearSounds();
    synth.addSound(new SineWaveSound());
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
    float lfoRate = *parameters.getRawParameterValue("LFO1_RATE");
    float phaseIncrement = lfoRate / getSampleRate();

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        lfoPhase1 += phaseIncrement;
        while (lfoPhase1 > 1.0f) {
            lfoPhase1 -= 1.0f;
        }

        lfoValue1 = lfoShape1.getValue(lfoPhase1);
    }
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

    float detuneA = *parameters.getRawParameterValue("A_DETUNE");
    float detuneB = *parameters.getRawParameterValue("B_DETUNE");
    float detuneC = *parameters.getRawParameterValue("C_DETUNE");
    float detuneD = *parameters.getRawParameterValue("D_DETUNE");

    float morphA = *parameters.getRawParameterValue("A_MORPH");
    float morphB = *parameters.getRawParameterValue("B_MORPH");
    float morphC = *parameters.getRawParameterValue("C_MORPH");
    float morphD = *parameters.getRawParameterValue("D_MORPH");

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

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new IntuitionAudioProcessor();
}
