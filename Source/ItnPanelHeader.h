/*
  ==============================================================================

    ItnPanelHeader.h
    Created: 3 Dec 2025 1:52:59pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class ItnPanelHeader : public juce::Component {
public:
    ItnPanelHeader(juce::AudioProcessorValueTreeState& vts, const juce::String title, bool includeToggle = true, const juce::String toggleParamName = "");

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    juce::Label title;

    juce::String toggleParamName;
    juce::ToggleButton toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> toggleAttachment;
};