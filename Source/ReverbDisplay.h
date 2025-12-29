/*
  ==============================================================================

    ReverbDisplay.h
    Created: 30 Oct 2025 6:37:22pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ModMatrix.h"
#include "ItnLookAndFeel.h"
#include "SliderComponent.h"
#include "ReverbGraph.h"


class ReverbDisplay : public juce::Component {
public:
    ReverbDisplay(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix);
    ~ReverbDisplay();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;

    ReverbGraph graph;

    juce::ToggleButton toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> toggleAttachment;

    SliderComponent damping, roomSize, rvbWidth, dryLevel, wetLevel;
};