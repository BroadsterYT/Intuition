/*
  ==============================================================================

    DelayDisplay.h
    Created: 14 Nov 2025 6:57:38pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ModMatrix.h"
#include "ItnLookAndFeel.h"
#include "ItnSlider.h"
#include "DelayGraph.h"


class DelayDisplay : public juce::Component {
public:
    DelayDisplay(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix);
    ~DelayDisplay();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;

    DelayGraph graph;

    juce::ToggleButton toggle;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> toggleAttachment;
    ItnSlider timeMs, feedback, cutoff, dry, wet;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> timeMsAttachment, feedbackAttachment, dryAttachment, wetAttachment, cutoffAttachment;

    juce::ToggleButton bpmSync;
};