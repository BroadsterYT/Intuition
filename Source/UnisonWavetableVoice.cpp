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
    WavetableBank& bankToUse,
    const juce::String octaveParamName,
    const juce::String coarseParamName,
    const juce::String fineParamName
) : parameters(vts),
    bank(bankToUse),
    octaveParamName(octaveParamName),
    coarseParamName(coarseParamName),
    fineParamName(fineParamName) {

    setUnison(unison);
    adsr.setSampleRate(getSampleRate());
    updateOscDetunes();
}

void UnisonWavetableVoice::setWavetable(WavetableBank& bankToUse) {
    bank = bankToUse;
}

int UnisonWavetableVoice::getUnison() {
    return unison;
}

void UnisonWavetableVoice::setUnison(int newUnison) {
    if (newUnison > oscs.size()) {
        for (int i = oscs.size(); i < newUnison; ++i) {
            WavetableOsc newOsc;
            newOsc.setParameters(&parameters);

            newOsc.setBank(bank);
            newOsc.setSampleRate(getSampleRate());
            oscs.push_back(newOsc);
        }
    }
    else if (newUnison < oscs.size()) {
        oscs.resize(newUnison);
    }

    unison = newUnison;
    updateOscDetunes();
}

float UnisonWavetableVoice::getDetuneRange() {
    return detuneRange;
}

void UnisonWavetableVoice::setDetuneRange(float range) {
    detuneRange = range;
    updateOscDetunes();
}

void UnisonWavetableVoice::updateOscDetunes() {
    detuneOffsets.resize(unison);
    
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
        int octave = static_cast<int>(*parameters.getRawParameterValue(octaveParamName));
        int semitone = static_cast<int>(*parameters.getRawParameterValue(coarseParamName));
        int fine = static_cast<int>(*parameters.getRawParameterValue(fineParamName));

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
    }
}

bool UnisonWavetableVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void UnisonWavetableVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*pitchWheel*/) {
    level = velocity;

    int octave = static_cast<int>(*parameters.getRawParameterValue(octaveParamName));
    int semitones = static_cast<int>(*parameters.getRawParameterValue(coarseParamName));
    int fine = static_cast<int>(*parameters.getRawParameterValue(fineParamName));

    currentFreq = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    for (int i = 0; i < unison; ++i) {
        oscs[i].setFrequency(
            currentFreq,
            octave,
            semitones,
            fine,
            detuneOffsets[i]
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