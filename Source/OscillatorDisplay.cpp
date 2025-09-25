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

    unison.setSliderStyle(juce::Slider::Rotary);
    detune.setSliderStyle(juce::Slider::Rotary);

    unison.setRange(1, 8, 1);
    detune.setRange(0, 100);

    unison.setTextBoxStyle(juce::Slider::NoTextBox, false, 20, 15);
    detune.setTextBoxStyle(juce::Slider::NoTextBox, false, 20, 15);

    addAndMakeVisible(unisonLabel);
    addAndMakeVisible(detuneLabel);

    unisonLabel.setText("Unison", juce::NotificationType::dontSendNotification);
    detuneLabel.setText("Detune", juce::NotificationType::dontSendNotification);

    unisonLabel.setJustificationType(juce::Justification::centredBottom);
    detuneLabel.setJustificationType(juce::Justification::centredBottom);
}

void OscillatorDisplay::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::darkgrey);
    g.fillRect(getLocalBounds());
}

void OscillatorDisplay::resized() {
    auto area = getLocalBounds().reduced(10);
    auto labelArea = area.removeFromBottom(15);

    int knobWidth = area.getWidth() / 2;
    unison.setBounds(area.removeFromLeft(knobWidth));
    detune.setBounds(area.removeFromLeft(knobWidth));

    unisonLabel.setBounds(labelArea.removeFromLeft(knobWidth));
    detuneLabel.setBounds(labelArea.removeFromLeft(knobWidth));
}