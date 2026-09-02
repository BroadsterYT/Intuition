/*
  ==============================================================================

    ConvoTextBubble.h
    Created: 9 Jan 2026 10:01:10am
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnLookAndFeel.h"
#include "TypewriterText.h"

/// <summary>
/// UI element representing a text bubble in an Intumi session
/// </summary>
class ConvoTextBubble : public juce::Component, private juce::Timer {
public:
    ConvoTextBubble(const juce::String& role, const juce::String& messageText, bool revealAll = false);
    ~ConvoTextBubble();

    float getMessageTextHeight() const;
    juce::String getRole() const;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    TypewriterText message;
    juce::String role;

    void timerCallback() override;
};
