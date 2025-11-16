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
#include "ItnSlider.h"
#include "ReverbGraph.h"


class ReverbDisplay : public juce::Component {
public:
    ReverbDisplay(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix);
    ~ReverbDisplay();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    ItnLookAndFeel lookAndFeel;
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;

    ReverbGraph graph;

    ItnSlider damping, roomSize, rvbWidth, dryLevel, wetLevel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dampingAttachment, roomSizeAttachment, rvbWidthAttachment, dryLevelAttachment, wetLevelAttachment;
};