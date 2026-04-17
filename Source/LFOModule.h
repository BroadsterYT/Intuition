/*
  ==============================================================================

    LFOModule.h
    Created: 16 Apr 2026 10:32:51am
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "LFOShape.h"
#include "TempoDivision.h"


class LFOModule {
public:
    LFOModule(juce::AudioProcessorValueTreeState& vts, const juce::String& modeName, const juce::String& syncDivName, const juce::String& rateName);
    void prepare(float sr);

    void updateLFOFrequency(float bpm);
    void updateLFOPhase(float bpm, double ppq, int numSamples);

    LFOShape* getShapePtr();
    float* getPhasePtr();

private:
    juce::AudioProcessorValueTreeState& parameters;
    float sampleRate = 44100.0f;

    // Parameters
    juce::String modeName = "DEFAULT";
    juce::String syncDivName = "DEFAULT";
    juce::String rateName = "DEFAULT";

    LFOShape shape;
    float phase = 0.0f;
    float rate = 0.0f;
    float lastValue = 0.0f;
};
