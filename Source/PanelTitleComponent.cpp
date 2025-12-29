/*
  ==============================================================================

    PanelTitleComponent.cpp
    Created: 16 Dec 2025 9:38:28pm
    Author:  BroDe

  ==============================================================================
*/

#include "PanelTitleComponent.h"

PanelTitleComponent::PanelTitleComponent(juce::AudioProcessorValueTreeState& vts, const juce::String displayTitle, bool buttonVisible, const juce::String toggleParamName
) : parameters(vts), buttonVisible(buttonVisible), button(vts, toggleParamName) {
    title.setComponentID("PanelTitle");
    title.setText(displayTitle, juce::dontSendNotification);
    addAndMakeVisible(title);

    if (buttonVisible) {
        addAndMakeVisible(button);
    }
}

void PanelTitleComponent::resized() {
    auto area = getLocalBounds();

    if (buttonVisible) {
        juce::Rectangle<int> buttonBounds;
        buttonBounds.setSize(60, 25);
        int togglePosY = (int)(0.5 * (area.getHeight() - buttonBounds.getHeight()));
        buttonBounds.setPosition(5, togglePosY);
        button.setBounds(buttonBounds);

        area.removeFromLeft(5 + buttonBounds.getWidth());  // Rest of area for title and whatever else
    }
    else {
        area.removeFromLeft(5);
    }

    // Label location
    title.setBounds(area);
}
