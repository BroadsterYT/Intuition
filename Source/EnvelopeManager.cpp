/*
  ==============================================================================

    EnvelopeManager.cpp
    Created: 24 Oct 2025 2:01:21pm
    Author:  BroDe

  ==============================================================================
*/

#include "EnvelopeManager.h"

EnvelopeManager::EnvelopeManager(juce::AudioProcessorValueTreeState& vts) : parameters(vts) {

}

void EnvelopeManager::prepare(double newSampleRate) {
    for (auto& env : envs) {
        env.setSampleRate(newSampleRate);
    }
}

void EnvelopeManager::setParameters() {
    float attackOsc = *parameters.getRawParameterValue("ENV_OSC_ATTACK");
    float decayOsc = *parameters.getRawParameterValue("ENV_OSC_DECAY");
    float sustainOsc = *parameters.getRawParameterValue("ENV_OSC_SUSTAIN");
    float releaseOsc = *parameters.getRawParameterValue("ENV_OSC_RELEASE");

    float attack1 = *parameters.getRawParameterValue("ENV1_ATTACK");
    float decay1 = *parameters.getRawParameterValue("ENV1_DECAY");
    float sustain1 = *parameters.getRawParameterValue("ENV1_SUSTAIN");
    float release1 = *parameters.getRawParameterValue("ENV1_RELEASE");

    float attack2 = *parameters.getRawParameterValue("ENV2_ATTACK");
    float decay2 = *parameters.getRawParameterValue("ENV2_DECAY");
    float sustain2 = *parameters.getRawParameterValue("ENV2_SUSTAIN");
    float release2 = *parameters.getRawParameterValue("ENV2_RELEASE");

    float attack3 = *parameters.getRawParameterValue("ENV3_ATTACK");
    float decay3 = *parameters.getRawParameterValue("ENV3_DECAY");
    float sustain3 = *parameters.getRawParameterValue("ENV3_SUSTAIN");
    float release3 = *parameters.getRawParameterValue("ENV3_RELEASE");

    envs[0].setParameters(attackOsc, decayOsc, sustainOsc, releaseOsc);
    envs[1].setParameters(attack1, decay1, sustain1, release1);
    envs[2].setParameters(attack2, decay2, sustain2, release2);
    envs[3].setParameters(attack3, decay3, sustain3, release3);
}

double EnvelopeManager::getSampleRate() const {
    return sampleRate;
}

void EnvelopeManager::noteOn(int noteNumber) {
    bool wasEmpty = activeNotes.empty();
    activeNotes.insert(noteNumber);

    if (wasEmpty) {
        for (auto& env : envs) {
            env.noteOn();
        }
    }
}

void EnvelopeManager::noteOff(int noteNumber) {
    activeNotes.erase(noteNumber);

    if (activeNotes.empty()) {
        for (auto& env : envs) {
            env.noteOff();
        }
    }
}

void EnvelopeManager::process(int numSamples) {
    for (int i = 0; i < numSamples; ++i) {
        lastValues[0] = envs[0].getNextSample();
        lastValues[1] = envs[1].getNextSample();
        lastValues[2] = envs[2].getNextSample();
        lastValues[3] = envs[3].getNextSample();
    }
}

float EnvelopeManager::getEnvValue(size_t index) const {
    jassert(index < envs.size());
    return lastValues[index];
}

const GlobalEnvelope& EnvelopeManager::getEnv(size_t index) {
    jassert(index < envs.size());
    return envs[index];
}
