/*
  ==============================================================================

    ConvoMessageComponent.h
    Created: 9 Jan 2026 10:01:10am
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnLookAndFeel.h"
#include "TypewriterText.h"


class ConvoMessageComponent : public juce::Component, private juce::Timer {
public:
    ConvoMessageComponent(const juce::String& role, const juce::String& messageText, bool revealAll = false);
    ~ConvoMessageComponent();

    float getMessageTextHeight() const;
    juce::String getRole() const;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    TypewriterText message;
    juce::String role;

    void timerCallback() override;
};