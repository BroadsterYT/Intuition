/*
  ==============================================================================

    ButtonComponent.h
    Created: 28 Dec 2025 7:29:16pm
    Author:  Brody Brazill

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnLookAndFeel.h"
#include "ItnTooltip.h"


class ButtonComponent : public juce::Component {
public:
    ButtonComponent(juce::AudioProcessorValueTreeState& vts, const juce::String paramName, bool showName = false, const juce::String displayName = "");
    ~ButtonComponent();

    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;

    juce::ToggleButton button;
    bool showName = false;
    juce::Label nameLabel;

    juce::String paramName;  // Parameter name for the button
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachment;
};
