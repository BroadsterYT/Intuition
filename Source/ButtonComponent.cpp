/*
  ==============================================================================

    ButtonComponent.cpp
    Created: 28 Dec 2025 7:29:16pm
    Author:  Brody Brazill

  ==============================================================================
*/

#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(juce::AudioProcessorValueTreeState& vts, const juce::String paramName, bool showName, const juce::String displayName
) : parameters(vts), showName(showName) {
    setLookAndFeel(&ItnLookAndFeel::getInstance());

    addAndMakeVisible(button);

    if (showName) {
        addAndMakeVisible(nameLabel);
        nameLabel.setComponentID("sliderNameLabel");
        nameLabel.setText(displayName, juce::dontSendNotification);
        nameLabel.setJustificationType(juce::Justification::centredLeft);
    }
}

ButtonComponent::~ButtonComponent() {
    setLookAndFeel(nullptr);
}

void ButtonComponent::resized() {
    auto area = getLocalBounds();
}
