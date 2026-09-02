/*
  ==============================================================================

    ConvoCanvas.cpp
    Created: 9 Jan 2026 10:30:15am
    Author:  BroDe

  ==============================================================================
*/

#include "ConvoCanvas.h"


ConvoCanvas::ConvoCanvas() {
    setSize(1000, 1200);
}

void ConvoCanvas::addMessage(const juce::String& role, const juce::String& messageText, bool createRevealed) {
    auto* newMessage = new ConvoTextBubble(role, messageText, createRevealed);
    
    addAndMakeVisible(newMessage);
    messages.add(newMessage);

    resized();
}

void ConvoCanvas::paint(juce::Graphics& g) {
    g.fillAll(MinimalStyle::bgPanel);
}

void ConvoCanvas::resized() {
    int numMessages = messages.size();

    float lastY = 0.0f;
    for (int i = 0; i < numMessages; ++i) {
        auto* msg = messages.getUnchecked(i);
        float textHeight = msg->getMessageTextHeight();
        float width = 300.0f;
        
        float msgHeight = 20.0f + textHeight;
        juce::Rectangle<float> msgBounds(0.0f, lastY, 20.0f + width, msgHeight);
        lastY += msgHeight;

        if (msg->getRole() == "user") {
            msgBounds.setX(getLocalBounds().getWidth() - width);
        }
        else if (msg->getRole() == "intumi") {
            msgBounds.setX(0.0f);
        }

        msg->setBounds(msgBounds.toNearestInt());
    }
}
