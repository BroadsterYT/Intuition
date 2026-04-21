/*
  ==============================================================================

    LFOModule.cpp
    Created: 16 Apr 2026 10:32:51am
    Author:  BroDe

  ==============================================================================
*/

#include "LFOModule.h"

LFOModule::LFOModule(juce::AudioProcessorValueTreeState& vts, const juce::String& modeName, const juce::String& syncDivName, const juce::String& rateName)
    : parameters(vts), modeName(modeName), syncDivName(syncDivName), rateName(rateName) {

}

void LFOModule::prepare(float sr) {
    sampleRate = sr;
}

void LFOModule::updateLFOFrequency(float bpm) {
    int mode = (int)*parameters.getRawParameterValue(modeName);
    if (mode == 0) {  // Free
        rate = *parameters.getRawParameterValue(rateName);
    }
    else if (mode == 1) {  // Synced
        int tempoIndex = (int)*parameters.getRawParameterValue(syncDivName);
        TempoDivision division = static_cast<TempoDivision>(tempoIndex);
        float beatsPerCycle = divisionToBeats(division);
        rate = (bpm / 60.0f) * beatsPerCycle;
    }
}

void LFOModule::updateLFOPhase(float bpm, double ppq, int numSamples) {
    int mode = (int)*parameters.getRawParameterValue(modeName);
    if (mode == 1) {
        int tempoIndex = (int)*parameters.getRawParameterValue(syncDivName);
        TempoDivision division = static_cast<TempoDivision>(tempoIndex);
        float beatsPerCycle = divisionToBeats(division);

        if (ppq > 0.0) {
            phase = static_cast<float>(fmod(ppq / beatsPerCycle, 1.0));
            lastValue = shape.getValue(phase);
        }
        else {  // DAW is considered idle or the playhead doesn't exist
            float phaseInc = bpm / (60.0f * beatsPerCycle * sampleRate);
            for (int sample = 0; sample < numSamples; ++sample) {
                phase += phaseInc;
                while (phase > 1.0f) phase -= 1.0f;
                lastValue = shape.getValue(phase);
            }
        }
    }
    else if (mode == 0) {
        float tempRate = *parameters.getRawParameterValue(rateName);
        float phaseInc = tempRate / sampleRate;
        for (int sample = 0; sample < numSamples; ++sample) {
            phase += phaseInc;
            while (phase > 1.0f) phase -= 1.0f;
            lastValue = shape.getValue(phase);
        }
    }
}

LFOShape& LFOModule::getShapeRef() {
    return shape;
}

float* LFOModule::getPhasePtr() {
    return &phase;
}

float* LFOModule::getValuePtr() {
    return &lastValue;
}
