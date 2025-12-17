/*
  ==============================================================================

    PanelTitleComponent.h
    Created: 16 Dec 2025 9:38:28pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "ItnLookAndFeel.h"


/// <summary>
/// Component that contains a label and an optional toggle. Used as a header for a GUI panel.
/// </summary>
class PanelTitleComponent : public juce::Component {
public:
    PanelTitleComponent(juce::AudioProcessorValueTreeState& vts, const juce::String displayTitle, bool toggleVisible = false, const juce::String toggleParamName = "");

    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;

    juce::Label title;

    bool toggleVisible = false;
    juce::ToggleButton toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> toggleAttachment;
};