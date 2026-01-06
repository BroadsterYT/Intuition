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
#include "TooltipController.h"


class ButtonComponent : public juce::Component {
public:
    /// <summary>
    /// Initializer for a ButtonComponentObject. Contains a ToggleButton, an optional label, and a tooltip.
    /// </summary>
    /// <param name="vts">The ValueTreeState to reference</param>
    /// <param name="paramName">The parameter key to link the button to</param>
    /// <param name="tooltipParamKey">The parameter key of the tooltip text to show for the tooltip</param>
    /// <param name="showName">Should the button's label be displayed?</param>
    /// <param name="displayName">The text to display in the button's label</param>
    ButtonComponent(juce::AudioProcessorValueTreeState& vts, const juce::String paramName,
        const juce::String tooltipParamKey = "DEFAULT", bool showName = false, const juce::String displayName = "");
    ~ButtonComponent();

    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;

    juce::ToggleButton button;
    bool showName = false;
    juce::Label nameLabel;

    TooltipController tooltipController;

    juce::String paramName;  // Parameter name for the button
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachment;
};
