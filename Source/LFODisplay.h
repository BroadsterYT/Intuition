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
#include "TempoSelect.h"


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

    TempoSelect syncDiv;
    juce::String modeName;

    juce::ComboBox mode;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modeAttachment;

    SliderComponent rate;

    void modeChanged();
};