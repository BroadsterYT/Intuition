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

bool GlobalEnvelope::isActive() {
    return env.isActive();
}