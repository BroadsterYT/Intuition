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
    /// <summary>
    /// Creates and initializes a new PanelTitleComponent object.
    /// </summary>
    /// <param name="vts">The value tree state of the audio processor</param>
    /// <param name="displayTitle">The text to show as the title</param>
    /// <param name="toggleVisible">Should the title contain a toggle for the content it is labeling?</param>
    /// <param name="toggleParamName">The parameter name of the toggle. Can be empty if the toggle is disabled.</param>
    PanelTitleComponent(juce::AudioProcessorValueTreeState& vts, const juce::String displayTitle, bool toggleVisible = false, const juce::String toggleParamName = "");

    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;

    juce::Label title;

    bool toggleVisible = false;
    juce::ToggleButton toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> toggleAttachment;
};