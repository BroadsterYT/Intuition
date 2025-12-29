/*
  ==============================================================================

    SliderComponent.cpp
    Created: 17 Dec 2025 10:13:03pm
    Author:  BroDe

  ==============================================================================
*/

#include "SliderComponent.h"

SliderComponent::SliderComponent(juce::AudioProcessorValueTreeState& vts, const juce::String paramName, const juce::String displayName, const juce::String units) : parameters(vts), paramName(paramName), units(units) {
    addAndMakeVisible(slider);
    addAndMakeVisible(nameLabel);
    addAndMakeVisible(valueBox);

    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    nameLabel.setComponentID("sliderNameLabel");
    valueBox.setComponentID("sliderValueBox");
    
    nameLabel.setText(displayName, juce::dontSendNotification);
    nameLabel.setJustificationType(juce::Justification::centred);
    valueBox.setEditable(true);
    valueBox.setJustificationType(juce::Justification::centred);
    valueBox.setText(formatValueWithUnit(slider.getValue()), juce::dontSendNotification);

    // Linking valueBox to slider
    valueBox.onEditorShow = [this] {
        if (auto* editor = valueBox.getCurrentTextEditor()) {
            editor->setInputRestrictions(0, "0123456789.-");
            editor->setJustification(juce::Justification::centred);

            editor->setText(editor->getText().upToLastOccurrenceOf(" ", false, false));
            editor->selectAll();
        }
    };
    valueBox.onEditorHide = [this] {
        auto val = valueBox.getText().getDoubleValue();
        slider.setValue(val, juce::sendNotificationSync);
        valueBox.setText(formatValueWithUnit(val), juce::dontSendNotification);
    };
    slider.onValueChange = [this] {
        auto val = slider.getValue();
        valueBox.setText(formatValueWithUnit(val), juce::dontSendNotification);
    };

    // Setting slider links
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, paramName, slider);
}

void SliderComponent::setModMatrix(ModMatrix* matrix) {
    slider.setModMatrix(matrix, paramName);
}

void SliderComponent::setRange(double newMinimum, double newMaximum, double newInterval) {
    slider.setRange(newMinimum, newMaximum, newInterval);
}

void SliderComponent::setSkewFactorFromMidPoint(double sliderValueToShowAtMidPoint) {
    slider.setSkewFactorFromMidPoint(sliderValueToShowAtMidPoint);
}

void SliderComponent::setCustomTooltipText(const juce::String parameterKey) {
    //slider.setCustomTooltipText(parameterKey);
}

void SliderComponent::resized() {
    auto area = getLocalBounds();

    auto valueBoxArea = area.removeFromBottom(15);
    valueBox.setBounds(valueBoxArea);
    auto nameArea = area.removeFromBottom(15);
    nameLabel.setBounds(nameArea);

    slider.setBounds(area);
}

juce::String SliderComponent::formatValueWithUnit(double v) {
    double clamped = juce::jlimit(slider.getMinimum(), slider.getMaximum(), v);
    double rounded = std::round(clamped * 100.0) / 100.0;
    auto formatted = juce::String(rounded);
    
    if (!units.isEmpty()) {
        formatted += " " + units;
    }
    return formatted;
}
