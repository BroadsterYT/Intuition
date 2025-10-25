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
    env.noteOn();
}

void GlobalEnvelope::noteOff() {
    env.noteOff();
}

float GlobalEnvelope::getNextSample() {
    return env.getNextSample();
}

bool GlobalEnvelope::isActive() const {
    return env.isActive();
}

void GlobalEnvelope::applyEnvelopeToBuffer(juce::AudioBuffer<float>& buffer, int sampleStart, int numSamples) {
    env.applyEnvelopeToBuffer(buffer, sampleStart, numSamples);
}