/*
  ==============================================================================

    OscillatorDisplay.h
    Created: 24 Sep 2025 10:57:20pm
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnSlider.h"
#include "WaveformDisplay.h"
#include "WavetableBank.h"


class OscillatorDisplay : public juce::Component {
public:
    OscillatorDisplay(juce::AudioProcessor* p, juce::AudioProcessorValueTreeState& vts, WavetableBank& bank);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::AudioProcessor* processor;
    juce::AudioProcessorValueTreeState& parameters;

    ItnSlider unison, detune, morph;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> unisonAttachment, detuneAttachment, morphAttachment;

    juce::TextButton selectWaveButton;
    WaveformDisplay waveDisplay;
};