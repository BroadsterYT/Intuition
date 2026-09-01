/*
  ==============================================================================

    ConvoDisplay.h
    Created: 9 Jan 2026 10:30:15am
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ConvoMessageComponent.h"
#include "ItnLookAndFeel.h"


/// <summary>
/// An interface displaying text bubbles with the conversation between the
/// user and Intumi
/// </summary>
class ConvoDisplay : public juce::Component {
public:
    ConvoDisplay();

    void addMessage(const juce::String& role, const juce::String& messageText, bool createRevealed = false);
    void clear();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::OwnedArray<ConvoMessageComponent> messages;
};