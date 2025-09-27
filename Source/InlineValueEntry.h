/*
  ==============================================================================

    InlineValueEntry.h
    Created: 25 Sep 2025 7:47:05pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class InlineValueEntry : public juce::Component {
public:
    InlineValueEntry(double currentValue) {
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

    void setValueChangeCallback(std::function<void(double)> callback) {
        valueChangeCallback = callback;
        setExitKeyLambdas();
    }

    template<typename ComponentType>
    void linkToComponent(ComponentType* component, std::function<void(ComponentType*, double)> setter) {
        if (component) {
            setValueChangeCallback([component, setter](double value) {
                setter(component, value);
            });
        }
    }

    void resized() override {
        editor.setBounds(getLocalBounds().reduced(4));
    }

private:
    juce::TextEditor editor;
    std::function<void(double)> valueChangeCallback;

    void setExitKeyLambdas() {
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
};