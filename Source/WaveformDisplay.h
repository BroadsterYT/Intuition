/*
  ==============================================================================

    WaveformDisplay.h
    Created: 22 Sep 2025 12:24:28am
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class WaveformDisplay : public juce::Component {
public:
    WaveformDisplay();

    void setWaveform(const juce::AudioBuffer<float>& buffer);
    void paint(juce::Graphics& g) override;

private:
    juce::Array<float> waveform;
};