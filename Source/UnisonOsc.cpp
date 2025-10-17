/*
  ==============================================================================

    UnisonOsc.cpp
    Created: 4 Oct 2025 11:33:46pm
    Author:  BroDe

  ==============================================================================
*/

#include "UnisonOsc.h"


UnisonOsc::UnisonOsc() {}

void UnisonOsc::setParameters(juce::AudioProcessorValueTreeState* vts) {
    parameters = vts;
}

void UnisonOsc::setBank(WavetableBank* newBank) {
    bank = newBank;
    setUnison(unison);
}

void UnisonOsc::setSampleRate(double newRate) {
    sampleRate = newRate;

}

void UnisonOsc::setFrequency(
    float frequency,
    int oct,
    int semitones,
    int cents,
    float detuneCents
) {
    if (!bank || bank->size() == 0 || bank->getWavetable(0).getNumSamples() == 0) return;
    for (auto& osc : oscillators) {
        osc.setFrequency(
            frequency,
            0,
            0,
            0,
            detuneCents
        );
    }

    octave = oct;
    coarse = semitones;
    fine = cents;
}

void UnisonOsc::setCurrentFrequency(float freq) {
    currentFreq = freq;
}

void UnisonOsc::setUnison(int newUnison) {
    if (newUnison > oscillators.size()) {
        for (int i = 0; i < newUnison; ++i) {
            WavetableOsc newOsc;
            newOsc.setParameters(parameters);

            newOsc.setBank(bank);
            newOsc.setSampleRate(sampleRate);
            oscillators.push_back(newOsc);
        }
    }
    else if (newUnison < oscillators.size()) {
        oscillators.resize(newUnison);
    }

    unison = newUnison;
    updateOscDetunes();
}

void UnisonOsc::setMorph(float alpha) {
    for (auto& osc : oscillators) {
        osc.setMorph(alpha);
    }
}

void UnisonOsc::setDetuneRange(float range) {
    detuneRange = range;
    updateOscDetunes();
}

void UnisonOsc::updateOscDetunes() {
    detuneOffsets.resize(unison);

    if (unison == 1) {
        detuneOffsets[0] = 0.0f;
    }
    else {
        for (int i = 0; i < unison; ++i) {
            float detune = -detuneRange + i * (2 * detuneRange / (unison - 1));
            detuneOffsets[i] = detune;
        }
    }

    if (currentFreq >= 0) {
        for (int i = 0; i < unison; ++i) {
            oscillators[i].setFrequency(
                currentFreq,
                octave,
                coarse,
                fine,
                detuneOffsets[i]
            );
        }
    }
}

void UnisonOsc::resetPhase() {
    for (auto& osc : oscillators) {
        osc.resetPhase();
    }
}

float UnisonOsc::getSample() {
    if (oscillators.size() == 0) {
        DBG("No oscillators in UnisonOsc!");
        return 0.0f;
    }

    float sum = 0.0f;
    for (int i = 0; i < unison; ++i) {
        sum += oscillators[i].getSample();
    }

    return sum /= unison;
}