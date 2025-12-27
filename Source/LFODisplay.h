/*
  ==============================================================================

    LFODisplay.h
    Created: 18 Oct 2025 7:25:19pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnSlider.h"
#include "LFOEditor.h"
#include "SliderComponent.h"


class LFODisplay : public juce::Component {
public:
    LFODisplay(
        juce::AudioProcessorValueTreeState& vts,
        LFOShape& shape,
        float* phase,
        const juce::String modeName,
        const juce::String rateName,
        const juce::String syncDivName
    );

    void paint(juce::Graphics& g);
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    LFOEditor editor;

    juce::String modeName, syncDivName;

    juce::ComboBox mode, syncDiv;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeAttachment, syncDivAttachment;

    SliderComponent rate;

    void modeChanged();
};