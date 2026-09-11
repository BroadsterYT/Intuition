/*
  ==============================================================================

    ConvoCanvas.cpp
    Created: 9 Jan 2026 10:30:15am
    Author:  BroDe

  ==============================================================================
*/

#include "ConvoCanvas.h"


ConvoCanvas::ConvoCanvas() {}

void ConvoCanvas::setMinCanvasHeight(int newMinHeight) {
    minCanvasHeight = newMinHeight;
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
    const float bubbleWidth = 300.0f;  // TODO: Allow adjusting TypewriterText max width
    const float bubbleInnerPad = 20.0f;

    float heightSum = 0.0f;
    for (int i = 0; i < numMessages; ++i) {
        auto* msg = messages.getUnchecked(i);
        float textHeight = msg->getMessageTextHeight();
        
        float msgHeight = bubbleInnerPad + textHeight;
        juce::Rectangle<float> msgBounds(0.0f, heightSum, bubbleInnerPad + bubbleWidth, msgHeight);
        heightSum += msgHeight;

        if (msg->getRole() == "user") {
            msgBounds.setX(getLocalBounds().getWidth() - bubbleWidth);
        }
        else if (msg->getRole() == "intumi") {
            msgBounds.setX(0.0f);
        }

        msg->setBounds(msgBounds.toNearestInt());
    }

    int adjHeight = heightSum < minCanvasHeight ? minCanvasHeight : heightSum;
    setSize(getWidth(), adjHeight);
}
