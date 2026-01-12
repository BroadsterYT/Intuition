/*
  ==============================================================================

    ConvoMessageComponent.cpp
    Created: 9 Jan 2026 10:01:10am
    Author:  BroDe

  ==============================================================================
*/

#include "ConvoMessageComponent.h"

ConvoMessageComponent::ConvoMessageComponent(const juce::String& role, const juce::String& messageText, bool revealAll) : role(role) {
    setLookAndFeel(&ItnLookAndFeel::getInstance());
    startTimerHz(10);

    addAndMakeVisible(message);
    message.setText(messageText, revealAll);
}

ConvoMessageComponent::~ConvoMessageComponent() {
    setLookAndFeel(nullptr);
    stopTimer();
}

float ConvoMessageComponent::getMessageTextHeight() const {
    return message.getFullTextHeight();
}

juce::String ConvoMessageComponent::getRole() const {
    return role;
}

void ConvoMessageComponent::paint(juce::Graphics& g) {
    MinimalStyle::drawCustomRoundedPanel(g, getLocalBounds().toFloat(), MinimalStyle::bgPanel);
}

void ConvoMessageComponent::resized() {
    auto area = getLocalBounds();
    message.setBounds(area.reduced(10));
    //message.setBounds(area);
}

void ConvoMessageComponent::timerCallback() {
    if (!message.iterateTypewriterEffect()) {
        stopTimer();
    }
}
