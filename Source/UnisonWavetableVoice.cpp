/*
  ==============================================================================

    UnisonWavetableVoice.cpp
    Created: 24 Sep 2025 3:33:29pm
    Author:  BroDe

  ==============================================================================
*/

#include "UnisonWavetableVoice.h"


UnisonWavetableVoice::UnisonWavetableVoice(
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

    setUnison();

    oscs[0].setBank(bank1);
    oscs[1].setBank(bank2);
    oscs[2].setBank(bank3);
    oscs[3].setBank(bank4);

    adsr.setSampleRate(getSampleRate());
    //updateOscDetunes();
}

int UnisonWavetableVoice::getUnison() {
    return unison;
}

UnisonOsc& UnisonWavetableVoice::getOsc(int index) {
    DBG("Getting: " << index << " Size: " << unison);
    return oscs[index];
}

void UnisonWavetableVoice::setUnison() {
    for (int i = oscs.size(); i < unison; ++i) {
        UnisonOsc newOsc;
        newOsc.setParameters(&parameters);

        newOsc.setBank(banks[i]);
        newOsc.setSampleRate(getSampleRate());
        oscs.push_back(newOsc);
    }

    //updateOscDetunes();
}

float UnisonWavetableVoice::getDetuneRange() {
    return detuneRange;
}

void UnisonWavetableVoice::setDetuneRange(float range) {
    detuneRange = range;
    //updateOscDetunes();
}

void UnisonWavetableVoice::updateOscDetunes() {
    /*detuneOffsets.resize(unison);
    
    if (unison <= 1) {
        detuneOffsets[0] = 0.0f;
    }
    else {
        for (int i = 0; i < unison; ++i) {
            float detune = -detuneRange + i * (2 * detuneRange / (unison - 1));
            detuneOffsets[i] = detune;
        }
    }

    if (currentFreq >= 0) {
        int octave = 0;//static_cast<int>(*parameters.getRawParameterValue(octaveParamName));
        int semitone = 0;//static_cast<int>(*parameters.getRawParameterValue(coarseParamName));
        int fine = 0;//static_cast<int>(*parameters.getRawParameterValue(fineParamName));

        // TODO: Make this a helper method
        for (int i = 0; i < unison; ++i) {
            oscs[i].setFrequency(
                currentFreq, 
                octave,
                semitone,
                fine,
                detuneOffsets[i]
            );
        }
    }*/
}

bool UnisonWavetableVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void UnisonWavetableVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*pitchWheel*/) {
    level = velocity;

    int octave = 0;//static_cast<int>(*parameters.getRawParameterValue(octaveParamName));
    int semitones = 0;//static_cast<int>(*parameters.getRawParameterValue(coarseParamName));
    int fine = 0;//static_cast<int>(*parameters.getRawParameterValue(fineParamName));

    currentFreq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    for (int i = 0; i < unison; ++i) {
        oscs[i].resetPhase();
        oscs[i].setCurrentFrequency(currentFreq);
        oscs[i].setFrequency(
            currentFreq,
            octave,
            semitones,
            fine,
            0.0f//detuneOffsets[i]
        );
    }
    adsr.noteOn();
}

void UnisonWavetableVoice::stopNote(float /*velocity*/, bool allowTailOff) {
    adsr.noteOff();
    currentFreq = -1.0f;

    if (!allowTailOff || !adsr.isActive()) {
        clearCurrentNote();
    }
}

void UnisonWavetableVoice::pitchWheelMoved(int) {}

void UnisonWavetableVoice::controllerMoved(int, int) {}

void UnisonWavetableVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    if (!adsr.isActive()) {
        clearCurrentNote();
        return;
    }

    for (int s = 0; s < numSamples; ++s) {
        float L = 0.0f;
        float R = 0.0f;

        for (int i = 0; i < unison; ++i) {
            float sample = oscs[i].getSample();

            // TODO: Implement panning
            L += sample;
            R += sample;
        }

        float adsrValue = adsr.getNextSample();
        float normal = 1.0f / (float)std::sqrt(unison);
        L *= normal * level * adsrValue;
        R *= normal * level * adsrValue;

        outputBuffer.addSample(0, startSample + s, L);
        outputBuffer.addSample(1, startSample + s, R);
    }
}

void UnisonWavetableVoice::setEnvelopeParams(const juce::ADSR::Parameters & params) {
    adsr.setParameters(params);
}