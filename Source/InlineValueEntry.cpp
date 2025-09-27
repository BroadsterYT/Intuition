/*
  ==============================================================================

    InlineValueEntry.cpp
    Created: 25 Sep 2025 7:47:15pm
    Author:  BroDe

  ==============================================================================
*/

#include "InlineValueEntry.h"


InlineValueEntry::InlineValueEntry(double currentValue) {
    addAndMakeVisible(editor);

    editor.setText(juce::String(currentValue));
    editor.setSelectAllWhenFocused(true);
    editor.setInputRestrictions(0, "0123456789.-"); // only numbers
    editor.setJustification(juce::Justification::centred);
    editor.setColour(juce::TextEditor::backgroundColourId, juce::Colours::black);
    editor.setColour(juce::TextEditor::outlineColourId, juce::Colours::grey);
    editor.setColour(juce::TextEditor::textColourId, juce::Colours::white);

    setExitKeyLambdas();
}

void InlineValueEntry::setValueChangeCallback(std::function<void(double)> callback) {
    valueChangeCallback = callback;
    setExitKeyLambdas();
}

void InlineValueEntry::resized() {
    editor.setBounds(getLocalBounds().reduced(4));
}

void InlineValueEntry::setExitKeyLambdas() {
    editor.onReturnKey = [this] {
        if (valueChangeCallback) {
            valueChangeCallback(editor.getText().getDoubleValue());
        }
        if (auto* cb = findParentComponentOfClass<juce::CallOutBox>()) {
            cb->exitModalState(1);  // Close popup
        };
    };

    editor.onEscapeKey = [this] {
        if (auto* cb = findParentComponentOfClass<juce::CallOutBox>()) {
            cb->exitModalState(0);  // Cancel
        };
    };
}