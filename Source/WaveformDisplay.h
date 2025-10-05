/*
  ==============================================================================

    WaveformDisplay.h
    Created: 22 Sep 2025 12:24:28am
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "WavetableBank.h"


class WaveformDisplay : public juce::Component, private juce::Timer {
public:
    WaveformDisplay(
        juce::AudioProcessorValueTreeState& vts,
        WavetableBank& bank,
        juce::String morphParamName
    );

    void setBank(WavetableBank& newBank);
    void paint(juce::Graphics& g) override;

private:
    WavetableBank& bank;
    juce::AudioProcessorValueTreeState& parameters;
    juce::Array<float> waveform;  // The values used to construct the image of the waveform

    juce::String morphParamName;

    void buildWaveform();
    void timerCallback() override;
};