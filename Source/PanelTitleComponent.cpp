/*
  ==============================================================================

    PanelTitleComponent.cpp
    Created: 16 Dec 2025 9:38:28pm
    Author:  BroDe

  ==============================================================================
*/

#include "PanelTitleComponent.h"

PanelTitleComponent::PanelTitleComponent(juce::AudioProcessorValueTreeState& vts, const juce::String displayTitle, bool toggleVisible, const juce::String toggleParamName) : parameters(vts), toggleVisible(toggleVisible) {
    title.setComponentID("PanelTitle");
    title.setText(displayTitle, juce::dontSendNotification);
    addAndMakeVisible(title);

    if (toggleVisible) {
        toggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(parameters, toggleParamName, toggle);
        addAndMakeVisible(toggle);
    }
}

void PanelTitleComponent::resized() {
    auto area = getLocalBounds();

    // Setting toggle button location
    juce::Rectangle<int> toggleBounds;
    toggleBounds.setSize(25, 25);
    int togglePosY = (int)(0.5 * (area.getHeight() - toggleBounds.getHeight()));
    toggleBounds.setPosition(5, togglePosY);
    toggle.setBounds(toggleBounds);

    // Label location
    area.removeFromLeft(5 + toggleBounds.getWidth());  // Rest of area for title and whatever else
    title.setBounds(area);
}
