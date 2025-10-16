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
    WavetableBank* bankToUse1,
    WavetableBank* bankToUse2,
    WavetableBank* bankToUse3,
    WavetableBank* bankToUse4
) : parameters(vts),
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

bool UnisonVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void UnisonVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*pitchWheel*/) {
    level = velocity;

    int octA = static_cast<int>(*parameters.getRawParameterValue("A_OCTAVE"));
    int semA = static_cast<int>(*parameters.getRawParameterValue("A_COARSE"));
    int finA = static_cast<int>(*parameters.getRawParameterValue("A_FINE"));

    int octB = static_cast<int>(*parameters.getRawParameterValue("B_OCTAVE"));
    int semB = static_cast<int>(*parameters.getRawParameterValue("B_COARSE"));
    int finB = static_cast<int>(*parameters.getRawParameterValue("B_FINE"));

    int octC = static_cast<int>(*parameters.getRawParameterValue("C_OCTAVE"));
    int semC = static_cast<int>(*parameters.getRawParameterValue("C_COARSE"));
    int finC = static_cast<int>(*parameters.getRawParameterValue("C_FINE"));

    int octD = static_cast<int>(*parameters.getRawParameterValue("D_OCTAVE"));
    int semD = static_cast<int>(*parameters.getRawParameterValue("D_COARSE"));
    int finD = static_cast<int>(*parameters.getRawParameterValue("D_FINE"));

    currentFreq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);

    oscA.resetPhase();
    oscB.resetPhase();
    oscC.resetPhase();
    oscD.resetPhase();

    oscA.setCurrentFrequency(currentFreq);
    oscB.setCurrentFrequency(currentFreq);
    oscC.setCurrentFrequency(currentFreq);
    oscD.setCurrentFrequency(currentFreq);

    oscA.setFrequency(
        currentFreq,
        octA,
        semA,
        finA,
        0.0f
    );
    oscB.setFrequency(
        currentFreq,
        octB,
        semB,
        finB,
        0.0f
    );
    oscC.setFrequency(
        currentFreq,
        octC,
        semC,
        finC,
        0.0f
    );
    oscD.setFrequency(
        currentFreq,
        octD,
        semD,
        finD,
        0.0f
    );

    adsr.noteOn();
}

void UnisonVoice::stopNote(float /*velocity*/, bool allowTailOff) {
    adsr.noteOff();
    currentFreq = -1.0f;

    if (!allowTailOff || !adsr.isActive()) {
        clearCurrentNote();
    }
}

void UnisonVoice::pitchWheelMoved(int) {}

void UnisonVoice::controllerMoved(int, int) {}

void UnisonVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    if (!adsr.isActive()) {
        clearCurrentNote();
        return;
    }

    bool toggleA = *parameters.getRawParameterValue("A_TOGGLE");
    bool toggleB = *parameters.getRawParameterValue("B_TOGGLE");
    bool toggleC = *parameters.getRawParameterValue("C_TOGGLE");
    bool toggleD = *parameters.getRawParameterValue("D_TOGGLE");

    for (int s = 0; s < numSamples; ++s) {
        float L = 0.0f;
        float R = 0.0f;

        // TODO: Implement panning
        if (toggleA) {
            float sampleA = oscA.getSample();
            L += sampleA;
            R += sampleA;
        }
        if (toggleB) {
            float sampleB = oscB.getSample();
            L += sampleB;
            R += sampleB;
        }
        if (toggleC) {
            float sampleC = oscC.getSample();
            L += sampleC;
            R += sampleC;
        }
        if (toggleD) {
            float sampleD = oscD.getSample();
            L += sampleD;
            R += sampleD;
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