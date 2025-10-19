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
    //currentFreq = -1.0f;

    if (!allowTailOff || !adsr.isActive()) {
        clearCurrentNote();
        currentMidiNote = -1;
    }
}

void UnisonVoice::pitchWheelMoved(int) {}

void UnisonVoice::controllerMoved(int, int) {}

void UnisonVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    if (!adsr.isActive()) {
        clearCurrentNote();
        return;
    }

    updatePitch();

    bool toggleA = *parameters.getRawParameterValue("A_TOGGLE");
    bool toggleB = *parameters.getRawParameterValue("B_TOGGLE");
    bool toggleC = *parameters.getRawParameterValue("C_TOGGLE");
    bool toggleD = *parameters.getRawParameterValue("D_TOGGLE");

    for (int s = 0; s < numSamples; ++s) {
        float L = 0.0f;
        float R = 0.0f;

        if (toggleA) {
            auto [l, r] = oscA.getSample();
            L += l;
            R += r;
        }
        if (toggleB) {
            auto [l, r] = oscB.getSample();
            L += l;
            R += r;
        }
        if (toggleC) {
            auto [l, r] = oscC.getSample();
            L += l;
            R += r;
        }
        if (toggleD) {
            auto [l, r] = oscD.getSample();
            L += l;
            R += r;
        }

        float adsrValue = adsr.getNextSample();
        L *= level * adsrValue;
        R *= level * adsrValue;

        outputBuffer.addSample(0, startSample + s, L);
        outputBuffer.addSample(1, startSample + s, R);
    }
}

void UnisonVoice::setEnvelopeParams(const juce::ADSR::Parameters & params) {
    adsr.setParameters(params);
}