/*
  ==============================================================================

    TempoSelect.cpp
    Created: 18 Feb 2026 9:50:24am
    Author:  BroDe

  ==============================================================================
*/

#include "TempoSelect.h"

TempoSelect::TempoSelect(juce::AudioProcessorValueTreeState& vts, const juce::String& paramName) : parameters(vts) {
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(vts, paramName, comboBox);
    addAndMakeVisible(comboBox);

    comboBox.addItem("4 Bars", 1);
    comboBox.addItem("2 Bars", 2);
    comboBox.addItem("1/1", 3);
    comboBox.addItem("1/2", 4);
    comboBox.addItem("1/4", 5);
    comboBox.addItem("1/8", 6);
    comboBox.addItem("1/16", 7);
    comboBox.addItem("1/32", 8);
    comboBox.addItem("1/64", 9);
    comboBox.setSelectedId(3);
}

void TempoSelect::resized() {
    comboBox.setBounds(getLocalBounds());
}