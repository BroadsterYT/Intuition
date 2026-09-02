/*
  ==============================================================================

    ConvoTextBubble.cpp
    Created: 9 Jan 2026 10:01:10am
    Author:  BroDe

  ==============================================================================
*/

#include "ConvoTextBubble.h"


ConvoTextBubble::ConvoTextBubble(const juce::String& role, const juce::String& messageText, bool revealAll) : role(role) {
    setLookAndFeel(&ItnLookAndFeel::getInstance());
    startTimerHz(60);

    addAndMakeVisible(message);
    message.setText(messageText, revealAll);
}

ConvoTextBubble::~ConvoTextBubble() {
    setLookAndFeel(nullptr);
    stopTimer();
}

float ConvoTextBubble::getMessageTextHeight() const {
    return message.getFullTextHeight();
}

juce::String ConvoTextBubble::getRole() const {
    return role;
}

void ConvoTextBubble::paint(juce::Graphics& g) {
    MinimalStyle::drawCustomRoundedPanel(g, getLocalBounds().toFloat(), MinimalStyle::bgPanel);
}

void ConvoTextBubble::resized() {
    auto area = getLocalBounds();
    message.setBounds(area.reduced(10));
}

void ConvoTextBubble::timerCallback() {
    if (!message.iterateTypewriterEffect()) {
        stopTimer();
    }
}
