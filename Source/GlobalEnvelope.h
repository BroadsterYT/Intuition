/*
  ==============================================================================

    GlobalEnvelope.h
    Created: 24 Oct 2025 2:06:01pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class GlobalEnvelope {
public:
    void setSampleRate(double newSampleRate);
    const juce::ADSR::Parameters& getParameters() const;
    void setParameters(float attack, float decay, float sustain, float release);
    
    void noteOn();
    void noteOff();

    float getNextSample();
    bool isActive() const;

private:
    juce::ADSR env;
    juce::ADSR::Parameters envParams;
    double sampleRate = 44100.0;
};