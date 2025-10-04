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
    WaveformDisplay(juce::AudioProcessorValueTreeState& vts, WavetableBank& bank);

    void setBank(WavetableBank& newBank);
    void paint(juce::Graphics& g) override;

private:
    WavetableBank& bank;
    juce::AudioProcessorValueTreeState& parameters;
    juce::Array<float> waveform;

    void buildWaveform();
    void timerCallback() override;
};