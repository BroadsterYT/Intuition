/*
  ==============================================================================

    ConvoViewport.h
    Created: 2 Sep 2026 4:47:54pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include "ConvoCanvas.h"
#include <JuceHeader.h>


/// <summary>
/// A ConvoCanvas component wrapped inside a juce::Viewport for 
/// scrolling capabilities.
/// </summary>
class ConvoViewport : public juce::Component {
public:
    ConvoViewport(const juce::String& convoId);

    juce::String getAssignedConvoId();
    void addMessage(
        const juce::String& role,
        const juce::String& messageText,
        bool createRevealed = false
    );

    void resized() override;

private:
    juce::String convoId;
    juce::Viewport viewport;
    ConvoCanvas canvas;
};
