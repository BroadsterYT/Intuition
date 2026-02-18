/*
  ==============================================================================

    TempoSelect.h
    Created: 18 Feb 2026 9:50:23am
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "TempoDivision.h"


class TempoSelect : public juce::Component {
public:
    TempoSelect(juce::AudioProcessorValueTreeState& vts, const juce::String& paramName);

    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;

    juce::ComboBox comboBox;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> attachment;
};
