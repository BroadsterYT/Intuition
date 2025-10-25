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
    EnvelopeManager& envManager,
    WavetableBank* bankToUse1,
    WavetableBank* bankToUse2,
    WavetableBank* bankToUse3,
    WavetableBank* bankToUse4
) : parameters(vts),
    modMatrix(matrix),
    envManager(envManager),
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

    //adsr.noteOn();
    //envManager.getEnv(0).
    envManager.noteOn(midiNoteNumber);
}

void UnisonVoice::stopNote(float /*velocity*/, bool allowTailOff) {
    //adsr.noteOff();
    envManager.noteOff(-1);

    if (!allowTailOff || !envManager.getEnv(0).isActive()) {
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
    bool envActive = envManager.getEnv(0).isActive();
    if (!envActive) {
        clearCurrentNote();
        return;
    }

    updatePitch();

    bool toggleA = *parameters.getRawParameterValue("A_TOGGLE");
    bool toggleB = *parameters.getRawParameterValue("B_TOGGLE");
    bool toggleC = *parameters.getRawParameterValue("C_TOGGLE");
    bool toggleD = *parameters.getRawParameterValue("D_TOGGLE");

    bool sendA = *parameters.getRawParameterValue("A_FILTER_SEND");
    bool sendB = *parameters.getRawParameterValue("B_FILTER_SEND");
    bool sendC = *parameters.getRawParameterValue("C_FILTER_SEND");
    bool sendD = *parameters.getRawParameterValue("D_FILTER_SEND");

    prepareFilter(getSampleRate(), outputBuffer.getNumSamples(), outputBuffer.getNumChannels());

    /*DBG("Num Channels: " << outputBuffer.getNumChannels());*/

    juce::AudioBuffer<float> filteredBuffer;
    filteredBuffer.setSize(2, numSamples);
    filteredBuffer.clear();

    std::vector<float> dryL(numSamples, 0.0f);
    std::vector<float> dryR(numSamples, 0.0f);

    auto processOscBlock = [&](UnisonOsc& osc, bool active, bool filtered, float vol) {
        if (!active) return;

        for (int s = 0; s < numSamples; ++s) {
            auto [l, r] = osc.getSample();
            float outputVol = level * vol;

            if (filtered) {
                filteredBuffer.addSample(0, s, l * outputVol);
                filteredBuffer.addSample(1, s, r * outputVol);
            }
            else {
                dryL[s] += l * outputVol;
                dryR[s] += r * outputVol;
            }
        }
    };

    float volumeA = modMatrix.getModdedDest("A_VOLUME");
    float volumeB = modMatrix.getModdedDest("B_VOLUME");
    float volumeC = modMatrix.getModdedDest("C_VOLUME");
    float volumeD = modMatrix.getModdedDest("D_VOLUME");

    processOscBlock(oscA, toggleA, sendA, volumeA);
    processOscBlock(oscB, toggleB, sendB, volumeB);
    processOscBlock(oscC, toggleC, sendC, volumeC);
    processOscBlock(oscD, toggleD, sendD, volumeD);

    // Filter block
    juce::dsp::AudioBlock<float> block(filteredBuffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    filter.process(context);

    for (int s = 0; s < numSamples; ++s) {
        adsr.getNextSample();  // Drains samples, allows note to be stopped
        float env = envManager.getEnvValue(0);
        float L = (filteredBuffer.getSample(0, s) + dryL[s]) * env;
        float R = (filteredBuffer.getSample(1, s) + dryR[s]) * env;

        outputBuffer.addSample(0, startSample + s, L);
        outputBuffer.addSample(1, startSample + s, R);
    }
    //adsr.applyEnvelopeToBuffer(outputBuffer, 0, numSamples);
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
