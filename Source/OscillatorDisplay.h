/*
  ==============================================================================

    OscillatorDisplay.h
    Created: 24 Sep 2025 10:57:20pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnSlider.h"


class OscillatorDisplay : public juce::Component {
public:
    OscillatorDisplay(juce::AudioProcessorValueTreeState& vts);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;

    ItnSlider unison, detune;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> unisonAttachment, detuneAttachment;
};