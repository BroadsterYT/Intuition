/*
  ==============================================================================

    GlobalEnvelope.cpp
    Created: 24 Oct 2025 2:06:01pm
    Author:  BroDe

  ==============================================================================
*/

#include "GlobalEnvelope.h"


void GlobalEnvelope::setSampleRate(double newSampleRate) {
    sampleRate = newSampleRate;
}

const juce::ADSR::Parameters& GlobalEnvelope::getParameters() const {
    return env.getParameters();
}

void GlobalEnvelope::setParameters(float attack, float decay, float sustain, float release) {
    envParams.attack = attack;
    envParams.decay = decay;
    envParams.sustain = sustain;
    envParams.release = release;
    env.setParameters(envParams);
}

void GlobalEnvelope::noteOn() {
    sampleIndex = 0;
    isNoteHeld = true;
    env.noteOn();
}

void GlobalEnvelope::noteOff() {
    isNoteHeld = false;
    env.noteOff();
}

float GlobalEnvelope::getNextSample() {
    if (!isSustain()) sampleIndex++;
    return env.getNextSample();
}

int GlobalEnvelope::getSampleIndex() const {
    return sampleIndex;
}

float GlobalEnvelope::getNormalizedTime() const {
    /*float totalSamples = (envParams.attack + envParams.decay + envParams.release) * sampleRate;
    if (totalSamples <= 0.0f) return 0.0f;
    return juce::jlimit(0.0f, 1.0f, sampleIndex / totalSamples);*/
    if (!env.isActive()) return 0.0f;

    float attackSamples = envParams.attack * sampleRate;
    float decaySamples = envParams.decay * sampleRate;
    float releaseSamples = envParams.release * sampleRate;
    float totalSamples = attackSamples + decaySamples + releaseSamples;

    if (totalSamples <= 0.0f) {
        return 0.0f;
    }

    float t = 0.0f;

    if (sampleIndex <= attackSamples) {
        t = sampleIndex / totalSamples;
    }
    else if (sampleIndex < (attackSamples + decaySamples)) {
        t = (attackSamples + (sampleIndex - attackSamples)) / totalSamples;
    }
    else if (isSustain()) {
        t = (attackSamples + decaySamples) / totalSamples;
    }
    else {
        int releaseSampleIndex = sampleIndex - (attackSamples + decaySamples);
        t = (attackSamples + decaySamples + releaseSampleIndex) / totalSamples;
    }

    return t;
}

bool GlobalEnvelope::isActive() const {
    return env.isActive();
}

void GlobalEnvelope::applyEnvelopeToBuffer(juce::AudioBuffer<float>& buffer, int sampleStart, int numSamples) {
    env.applyEnvelopeToBuffer(buffer, sampleStart, numSamples);
}

bool GlobalEnvelope::isSustain() const {
    float attackSamples = envParams.attack * sampleRate;
    float decaySamples = envParams.decay * sampleRate;

    return env.isActive() && isNoteHeld && sampleIndex >= (attackSamples + decaySamples);
}
