/*
  ==============================================================================

    ButtonComponent.cpp
    Created: 28 Dec 2025 7:29:16pm
    Author:  Brody Brazill

  ==============================================================================
*/

#include "ButtonComponent.h"

ButtonComponent::ButtonComponent(juce::AudioProcessorValueTreeState& vts, const juce::String paramName, const juce::String tooltipParamKey, bool showName, const juce::String displayName
) : parameters(vts), paramName(paramName), tooltipController(button), showName(showName) {
    setLookAndFeel(&ItnLookAndFeel::getInstance());
    tooltipController.setTooltipText(tooltipParamKey);

    addAndMakeVisible(button);
    attachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(parameters, paramName, button);

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
    auto buttonArea = area.removeFromLeft(25);

    button.setBounds(buttonArea);
    nameLabel.setBounds(area);
}