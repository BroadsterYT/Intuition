/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
        
        std::make_unique<juce::AudioParameterInt>("A_UNISON", "A Unison", 1, 8, 1),
        std::make_unique<juce::AudioParameterFloat>("A_DETUNE", "A Detune", 0, 100, 0),
        std::make_unique<juce::AudioParameterFloat>("A_MORPH", "A Morph", 0.0f, 1.0f, 0.0f),

        std::make_unique<juce::AudioParameterFloat>("LFO1_RATE", "LFO 1 Rate", 0.0f, 30.0f, 1.0f),
        std::make_unique<juce::AudioParameterFloat>("LFO1_DEPTH", "LFO 1 Depth", 0.0f, 1.0f, 0.25f),
    })
{
    parameters.state = juce::ValueTree("PARAMETERS");
}

IntuitionAudioProcessor::~IntuitionAudioProcessor()
{
}

void IntuitionAudioProcessor::addWavetableToBank1(juce::File& wavFile) {
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    jassert(wavFile.existsAsFile());
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(wavFile));

    if (reader != nullptr) {
        juce::AudioBuffer<float> temp;
        temp.setSize((int)reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&temp, 0, (int)reader->lengthInSamples, 0, true, true);
        
        bank1.addWavetable(temp);
        DBG("Bank size: " << bank1.size());

        synth.clearVoices();
        for (int i = 0; i < 8; ++i) {
            synth.addVoice(new UnisonWavetableVoice(parameters, bank1));
        }
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

    juce::File file("C:/Users/BroDe/Downloads/AKWF/AKWF_cello/AKWF_cello_0001.wav");
    addWavetableToBank1(file);

    synth.clearVoices();
    for (int i = 0; i < 8; ++i) {
        synth.addVoice(new UnisonWavetableVoice(parameters, bank1));
    }
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

    juce::ADSR::Parameters adsrParams;
    adsrParams.attack = *parameters.getRawParameterValue("ATTACK");
    adsrParams.decay = *parameters.getRawParameterValue("DECAY");
    adsrParams.sustain = *parameters.getRawParameterValue("SUSTAIN");
    adsrParams.release = *parameters.getRawParameterValue("RELEASE");

    int unison = static_cast<int>(*parameters.getRawParameterValue("A_UNISON"));
    float detune = *parameters.getRawParameterValue("A_DETUNE");

    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto* v = dynamic_cast<UnisonWavetableVoice*>(synth.getVoice(i))) {
            v->setEnvelopeParams(adsrParams);

            v->setUnison(unison);
            v->setDetuneRange(detune);
        }
    }

    float lfoRate = *parameters.getRawParameterValue("LFO1_RATE");
    float lfoDepth = *parameters.getRawParameterValue("LFO1_DEPTH");
    float phaseIncrement = lfoRate / getSampleRate();
    float lfo1Value = 0.0f;
    
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
        lfo1Phase += phaseIncrement;
        while (lfo1Phase >= 1.0f) {
            lfo1Phase -= 1.0f;
        }

        lfo1Value = lfo1Shape.getValue(lfo1Phase);
    }

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

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
