/*
  ==============================================================================

    OscillatorDisplay.cpp
    Created: 24 Sep 2025 10:57:29pm
    Author:  BroDe

  ==============================================================================
*/

#include "OscillatorDisplay.h"

OscillatorDisplay::OscillatorDisplay(juce::AudioProcessorValueTreeState& vts) : parameters(vts) {
    addAndMakeVisible(unison);
    addAndMakeVisible(detune);

    unisonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "A_UNISON", unison);
    detuneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "A_DETUNE", detune);

    unison.setRange(1, 8, 1);
    detune.setRange(0, 100);

    unison.setLabelNames("Unison", "U");
    detune.setLabelNames("Detune", "D");

    unison.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    detune.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
}

void OscillatorDisplay::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::darkgrey);
    g.fillRect(getLocalBounds());
}

void OscillatorDisplay::resized() {
    auto area = getLocalBounds().reduced(10);

    int knobWidth = area.getWidth() / 2;
    unison.setBounds(area.removeFromLeft(knobWidth));
    detune.setBounds(area.removeFromLeft(knobWidth));
}