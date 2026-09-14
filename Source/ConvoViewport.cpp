/*
  ==============================================================================

    ConvoViewport.cpp
    Created: 2 Sep 2026 4:47:54pm
    Author:  BroDe

  ==============================================================================
*/

#include "ConvoViewport.h"


ConvoViewport::ConvoViewport(const juce::String& convoId) : convoId(convoId) {
    viewport.setViewedComponent(&canvas);
    viewport.setScrollBarsShown(true, false);
    addAndMakeVisible(viewport);
}

juce::String ConvoViewport::getAssignedConvoId() {
    return convoId;
}

void ConvoViewport::addMessage(
    const juce::String& role,
    const juce::String& messageText,
    bool createRevealed
) {
    canvas.addMessage(role, messageText, createRevealed);
}

void ConvoViewport::resized() {
    auto area = getLocalBounds();
    viewport.setBounds(getLocalBounds());
    canvas.setBounds(getLocalBounds());
}
