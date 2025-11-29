/*
  ==============================================================================

    UnisonVoice.cpp
    Created: 24 Sep 2025 3:33:29pm
    Author:  BroDe

  ==============================================================================
*/

#include "UnisonVoice.h"


UnisonVoice::UnisonVoice(
    juce::AudioProcessorValueTreeState& vts,
    ModMatrix& matrix,
    WavetableBank* bankToUse1,
    WavetableBank* bankToUse2,
    WavetableBank* bankToUse3,
    WavetableBank* bankToUse4
) : parameters(vts),
    modMatrix(matrix),
    bank1(bankToUse1), 
    bank2(bankToUse2), 
    bank3(bankToUse3), 
    bank4(bankToUse4)
{
    oscA.setParameters(&parameters);
    oscB.setParameters(&parameters);
    oscC.setParameters(&parameters);
    oscD.setParameters(&parameters);

    oscA.setBank(bank1);
    oscB.setBank(bank2);
    oscC.setBank(bank3);
    oscD.setBank(bank4);

    oscA.setSampleRate(getSampleRate());
    oscB.setSampleRate(getSampleRate());
    oscC.setSampleRate(getSampleRate());
    oscD.setSampleRate(getSampleRate());

    adsr.setSampleRate(getSampleRate());
}

UnisonOsc& UnisonVoice::getOscA() {
    return oscA;
}

UnisonOsc& UnisonVoice::getOscB() {
    return oscB;
}

UnisonOsc& UnisonVoice::getOscC() {
    return oscC;
}

UnisonOsc& UnisonVoice::getOscD() {
    return oscD;
}

void UnisonVoice::updatePitch() {
    if (currentMidiNote >= 0) {
        float baseFreq = juce::MidiMessage::getMidiNoteInHertz(currentMidiNote);

        int octA = (int)modMatrix.getModdedDest("A_OCTAVE");
        int semA = (int)modMatrix.getModdedDest("A_COARSE");
        int finA = (int)modMatrix.getModdedDest("A_FINE");

        int octB = (int)modMatrix.getModdedDest("B_OCTAVE");
        int semB = (int)modMatrix.getModdedDest("B_COARSE");
        int finB = (int)modMatrix.getModdedDest("B_FINE");

        int octC = (int)modMatrix.getModdedDest("C_OCTAVE");
        int semC = (int)modMatrix.getModdedDest("C_COARSE");
        int finC = (int)modMatrix.getModdedDest("C_FINE");

        int octD = (int)modMatrix.getModdedDest("D_OCTAVE");
        int semD = (int)modMatrix.getModdedDest("D_COARSE");
        int finD = (int)modMatrix.getModdedDest("D_FINE");

        oscA.setCurrentFrequency(baseFreq);
        oscB.setCurrentFrequency(baseFreq);
        oscC.setCurrentFrequency(baseFreq);
        oscD.setCurrentFrequency(baseFreq);

        oscA.setFrequency(baseFreq, octA, semA, finA, 0.0f);
        oscB.setFrequency(baseFreq, octB, semB, finB, 0.0f);
        oscC.setFrequency(baseFreq, octC, semC, finC, 0.0f);
        oscD.setFrequency(baseFreq, octD, semD, finD, 0.0f);

        oscA.updateOscDetunes();
        oscB.updateOscDetunes();
        oscC.updateOscDetunes();
        oscD.updateOscDetunes();
    }
}

bool UnisonVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void UnisonVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*pitchWheel*/) {
    level = velocity;

    const double sr = getSampleRate();
    if (sr > 0.0) {
        oscA.setSampleRate(sr);
        oscB.setSampleRate(sr);
        oscC.setSampleRate(sr);
        oscD.setSampleRate(sr);
    }

    currentMidiNote = midiNoteNumber;
    currentFreq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);

    oscA.resetPhase();
    oscB.resetPhase();
    oscC.resetPhase();
    oscD.resetPhase();

    oscA.setRandomPhaseOffsets();
    oscB.setRandomPhaseOffsets();
    oscC.setRandomPhaseOffsets();
    oscD.setRandomPhaseOffsets();

    oscA.setCurrentFrequency(currentFreq);
    oscB.setCurrentFrequency(currentFreq);
    oscC.setCurrentFrequency(currentFreq);
    oscD.setCurrentFrequency(currentFreq);

    adsr.noteOn();
}

void UnisonVoice::stopNote(float /*velocity*/, bool allowTailOff) {
    adsr.noteOff();

    if (!allowTailOff || !adsr.isActive()) {
        clearCurrentNote();
        currentMidiNote = -1;
    }
}

void UnisonVoice::pitchWheelMoved(int) {}

void UnisonVoice::controllerMoved(int, int) {}

void UnisonVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels) {
    prepareFilter(sampleRate, samplesPerBlock, numChannels);
    filter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
}

void UnisonVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    if (!adsr.isActive()) {
        clearCurrentNote();
        return;
    }
    updatePitch();

    processAtHighRate(outputBuffer, startSample, numSamples);
}

void UnisonVoice::setFilterCutoff(float cutoff) {
    filter.setCutoffFrequency(cutoff);
}

void UnisonVoice::setFilterResonance(float res) {
    filter.setResonance(res);
}

void UnisonVoice::setFilterType(int type) {
    juce::dsp::StateVariableTPTFilterType filterType = juce::dsp::StateVariableTPTFilterType::lowpass;
    switch (type) {
    case 0:
        filterType = juce::dsp::StateVariableTPTFilterType::lowpass;
        break;
    case 1:
        filterType = juce::dsp::StateVariableTPTFilterType::highpass;
        break;
    case 2:
        filterType = juce::dsp::StateVariableTPTFilterType::bandpass;
        break;
    }

    filter.setType(filterType);
}

void UnisonVoice::setEnvelopeParams(const juce::ADSR::Parameters & params) {
    adsr.setParameters(params);
}

void UnisonVoice::prepareFilter(double sampleRate, int samplesPerBlock, int numChannels){
    if (filterPreparedChannels != numChannels) {
        juce::dsp::ProcessSpec pspec;
        pspec.sampleRate = sampleRate;
        pspec.maximumBlockSize = samplesPerBlock;
        pspec.numChannels = numChannels;
        filter.prepare(pspec);

        filterPreparedChannels = numChannels;
        DBG("Filter prepared.");
    }
}

void UnisonVoice::processAtHighRate(juce::AudioBuffer<float>& block, int startSample, int numSamples) {
    std::vector<float> dryL(numSamples, 0.0f);
    std::vector<float> dryR(numSamples, 0.0f);
    juce::AudioBuffer<float> filteredBuffer;

    prepareFilter(getSampleRate(), block.getNumSamples(), block.getNumChannels());
    filteredBuffer.setSize(2, numSamples);
    filteredBuffer.clear();

    //DBG("block samples: " << block.getNumSamples());
    //DBG("arg samples: " << numSamples);

    bool toggleA = *parameters.getRawParameterValue("A_TOGGLE");
    bool toggleB = *parameters.getRawParameterValue("B_TOGGLE");
    bool toggleC = *parameters.getRawParameterValue("C_TOGGLE");
    bool toggleD = *parameters.getRawParameterValue("D_TOGGLE");

    bool sendA = *parameters.getRawParameterValue("A_FILTER_SEND");
    bool sendB = *parameters.getRawParameterValue("B_FILTER_SEND");
    bool sendC = *parameters.getRawParameterValue("C_FILTER_SEND");
    bool sendD = *parameters.getRawParameterValue("D_FILTER_SEND");

    float volumeA = modMatrix.getModdedDest("A_VOLUME");
    float volumeB = modMatrix.getModdedDest("B_VOLUME");
    float volumeC = modMatrix.getModdedDest("C_VOLUME");
    float volumeD = modMatrix.getModdedDest("D_VOLUME");

    float panA = modMatrix.getModdedDest("A_PAN");
    float panB = modMatrix.getModdedDest("B_PAN");
    float panC = modMatrix.getModdedDest("C_PAN");
    float panD = modMatrix.getModdedDest("D_PAN");

    auto processOscBlock = [&](UnisonOsc& osc, bool active, bool filtered, float vol, float pan) {
        if (!active) return;

        for (int s = 0; s < (int)numSamples; ++s) {
            auto [l, r] = osc.getSample();
            float outputVol = level * vol;

            float panVal = juce::jmap(pan, 0.0f, 1.0f, -1.0f, 1.0f);
            float angle = (panVal + 1.0f) * juce::MathConstants<float>::pi / 4.0f;
            float panL = std::cos(angle);
            float panR = std::sin(angle);

            if (filtered) {
                filteredBuffer.addSample(0, s, l * outputVol * panL);
                filteredBuffer.addSample(1, s, r * outputVol * panR);
            }
            else {
                dryL[s] += l * outputVol * panL;
                dryR[s] += r * outputVol * panR;
            }
        }
    };

    processOscBlock(oscA, toggleA, sendA, volumeA, panA);
    processOscBlock(oscB, toggleB, sendB, volumeB, panB);
    processOscBlock(oscC, toggleC, sendC, volumeC, panC);
    processOscBlock(oscD, toggleD, sendD, volumeD, panD);

    // Filter block
    juce::dsp::AudioBlock<float> filterBlock(filteredBuffer);
    juce::dsp::ProcessContextReplacing<float> filterContext(filterBlock);
    filter.process(filterContext);

    /*auto left = block.getChannelPointer(0);
    auto right = block.getNumChannels() > 1 ? block.getChannelPointer(1) : nullptr;*/

    for (int s = 0; s < (int)numSamples; ++s) {
        float env = adsr.getNextSample();
        float L = (filteredBuffer.getSample(0, s) + dryL[s]) * env;
        float R = (filteredBuffer.getSample(1, s) + dryR[s]) * env;

        block.addSample(0, startSample + s, L);
        block.addSample(1, startSample + s, R);
    }
}
