/*
  ==============================================================================

    ConvoCanvas.h
    Created: 9 Jan 2026 10:30:15am
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ConvoTextBubble.h"
#include "ItnLookAndFeel.h"


/// <summary>
/// A UI displaying text bubbles with the conversation between the
/// user and Intumi.
/// </summary>
class ConvoCanvas : public juce::Component {
public:
    ConvoCanvas();

    void addMessage(const juce::String& role, const juce::String& messageText, bool createRevealed = false);
    void clear();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::OwnedArray<ConvoTextBubble> messages;
};