/*
  ==============================================================================

    FilterDisplay.h
    Created: 19 Oct 2025 3:36:37pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include "FilterGraph.h"
#include "ItnSlider.h"


class FilterDisplay : public juce::Component {
public:
    FilterDisplay(
        juce::AudioProcessorValueTreeState& vts,
        ModMatrix* modMatrix
    );

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;
    FilterGraph graph;

    ItnSlider frequency, resonance;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> frequencyAttachment, resonanceAttachment;

    juce::ToggleButton toggleA, toggleB, toggleC, toggleD;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> aToggleAttach, bToggleAttach, cToggleAttach, dToggleAttach;

    juce::ComboBox type;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> typeAttachment;
};