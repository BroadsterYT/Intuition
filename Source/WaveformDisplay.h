/*
  ==============================================================================

    WaveformDisplay.h
    Created: 22 Sep 2025 12:24:28am
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ModMatrix.h"
#include "WavetableBank.h"
#include "ItnLookAndFeel.h"


class WaveformDisplay : public juce::Component, private juce::Timer {
public:
    WaveformDisplay(
        juce::AudioProcessorValueTreeState& vts,
        ModMatrix* modMatrix,
        WavetableBank& bank,
        juce::String morphParamName
    );
    ~WaveformDisplay() override;

    void setBank(WavetableBank& newBank);
    void paint(juce::Graphics& g) override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;

    WavetableBank& bank;
    juce::Array<float> waveform;  // The values used to construct the image of the waveform

    juce::String morphParamName;

    float lastAlpha = 0.0f;
    bool needsRedraw();

    void buildWaveform();
    void timerCallback() override;
};