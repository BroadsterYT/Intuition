/*
  ==============================================================================

    DelayDisplay.cpp
    Created: 14 Nov 2025 6:57:38pm
    Author:  BroDe

  ==============================================================================
*/

#include "DelayDisplay.h"

DelayDisplay::DelayDisplay(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix
) : parameters(vts), modMatrix(modMatrix), graph(vts, modMatrix),
    timeMs(vts, "DELAY_TIME_MS", "DELAY TIME", "ms"),
    feedback(vts, "DELAY_FEEDBACK", "FEEDBACK"),
    cutoff(vts, "DELAY_CUTOFF", "CUTOFF FREQUENCY", "Hz"),
    dry(vts, "DELAY_DRY_LEVEL", "DRY"),
    wet(vts, "DELAY_WET_LEVEL", "WET") {
    setLookAndFeel(&ItnLookAndFeel::getInstance());

    addAndMakeVisible(toggle);
    addAndMakeVisible(timeMs);
    addAndMakeVisible(feedback);
    addAndMakeVisible(cutoff);
    addAndMakeVisible(dry);
    addAndMakeVisible(wet);
    addAndMakeVisible(graph);

    toggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(parameters, "DELAY_TOGGLE", toggle);

    timeMs.setModMatrix(modMatrix);
    feedback.setModMatrix(modMatrix);
    cutoff.setModMatrix(modMatrix);
    dry.setModMatrix(modMatrix);
    wet.setModMatrix(modMatrix);

    cutoff.setSkewFactorFromMidPoint(1000.0f);

    timeMs.setCustomTooltipText("DELAY_TIME_MS");
    feedback.setCustomTooltipText("DELAY_FEEDBACK");
    cutoff.setCustomTooltipText("DELAY_CUTOFF");
    dry.setCustomTooltipText("DELAY_DRY_LEVEL");
    wet.setCustomTooltipText("DELAY_WET_LEVEL");
}

DelayDisplay::~DelayDisplay() {
    setLookAndFeel(nullptr);
}

void DelayDisplay::paint(juce::Graphics& g) {
    g.fillAll(MinimalStyle::charcoal);
}

void DelayDisplay::resized() {
    auto area = getLocalBounds().reduced(10);

    toggle.setBounds(area.removeFromLeft(25));

    int knobWidth = area.getWidth() / 6;
    timeMs.setBounds(area.removeFromLeft(knobWidth));
    feedback.setBounds(area.removeFromLeft(knobWidth));
    cutoff.setBounds(area.removeFromLeft(knobWidth));
    dry.setBounds(area.removeFromLeft(knobWidth));
    wet.setBounds(area.removeFromLeft(knobWidth));

    graph.setBounds(area.removeFromLeft(knobWidth));
}
