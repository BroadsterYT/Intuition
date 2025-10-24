/*
  ==============================================================================

    EnvelopeManager.cpp
    Created: 24 Oct 2025 2:01:21pm
    Author:  BroDe

  ==============================================================================
*/

#include "EnvelopeManager.h"

EnvelopeManager::EnvelopeManager(juce::AudioProcessorValueTreeState& vts) : parameters(vts) {
    //float a1 = *parameters.getRawParameterValue("ATTACK")
    
    //envs[0].setParameters()
}

void EnvelopeManager::prepare(double newSampleRate) {
    for (auto& env : envs) {
        env.setSampleRate(newSampleRate);
    }
}

void EnvelopeManager::setParameters() {
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

    envs[0].setParameters(attack1, decay1, sustain1, release1);
    envs[1].setParameters(attack2, decay2, sustain2, release2);
    envs[2].setParameters(attack3, decay3, sustain3, release3);
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
    }
}

float EnvelopeManager::getEnvValue(size_t index) {
    jassert(index < envs.size());
    return lastValues[index];
}