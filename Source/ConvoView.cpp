/*
  ==============================================================================

    ConvoView.cpp
    Created: 9 Jan 2026 10:30:15am
    Author:  BroDe

  ==============================================================================
*/

#include "ConvoView.h"

ConvoView::ConvoView() {
    startTimerHz(60);
    setSize(1000, 1200);
}

ConvoView::~ConvoView() {
    stopTimer();
}

void ConvoView::addMessage(const juce::String& role, const juce::String& messageText, bool createRevealed) {
    auto* newMessage = new ConvoMessageComponent(role, messageText, createRevealed);
    
    addAndMakeVisible(newMessage);
    messages.add(newMessage);

    resized();
}

void ConvoView::paint(juce::Graphics& g) {
    g.fillAll(MinimalStyle::bgPanel);
}

void ConvoView::resized() {
    int numMessages = messages.size();

    float lastY = 0.0f;
    for (int i = 0; i < numMessages; ++i) {
        auto* msg = messages.getUnchecked(i);
        float textHeight = msg->getMessageTextHeight();
        float width = 300.0f;
        
        juce::Rectangle<float> msgBounds;
        float msgHeight = 20.0f + textHeight;
        msgBounds.setWidth(width);
        msgBounds.setHeight(msgHeight);
        msgBounds.setY(lastY);
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

void ConvoView::timerCallback() {
}
