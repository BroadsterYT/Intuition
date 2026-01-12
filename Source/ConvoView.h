/*
  ==============================================================================

    ConvoView.h
    Created: 9 Jan 2026 10:30:15am
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ConvoMessageComponent.h"
#include "ItnLookAndFeel.h"


class ConvoView : public juce::Component {
public:
    ConvoView();

    void addMessage(const juce::String& role, const juce::String& messageText, bool createRevealed = false);
    void clear();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::OwnedArray<ConvoMessageComponent> messages;
};