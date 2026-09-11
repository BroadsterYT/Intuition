/*
  ==============================================================================

    ConvoViewport.cpp
    Created: 2 Sep 2026 4:47:54pm
    Author:  BroDe

  ==============================================================================
*/

#include "ConvoViewport.h"


ConvoViewport::ConvoViewport() {
    viewport.setViewedComponent(&canvas);
    viewport.setScrollBarsShown(true, false);
    addAndMakeVisible(viewport);
}

void ConvoViewport::addMessage(
    const juce::String& role,
    const juce::String& messageText,
    bool createRevealed
) {
    canvas.addMessage(role, messageText, createRevealed);
}

void ConvoViewport::resized() {}

void ConvoViewport::setBounds(int x, int y, int width, int height) {
    Component::setBounds(x, y, width, height);
    viewport.setBounds(0, 0, width, height);

    canvas.setMinCanvasHeight(height);
    canvas.setBounds(0, 0, width, height);
}
