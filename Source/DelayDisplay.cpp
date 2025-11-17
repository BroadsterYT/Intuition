/*
  ==============================================================================

    DelayDisplay.cpp
    Created: 14 Nov 2025 6:57:38pm
    Author:  BroDe

  ==============================================================================
*/

#include "DelayDisplay.h"

DelayDisplay::DelayDisplay(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix
) : parameters(vts), modMatrix(modMatrix), graph(vts, modMatrix) {
    setLookAndFeel(&lookAndFeel);

    addAndMakeVisible(timeMs);
    addAndMakeVisible(feedback);
    addAndMakeVisible(wet);
    addAndMakeVisible(cutoff);
    addAndMakeVisible(graph);

    timeMsAttachment = std::make_unique< juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "DELAY_TIME_MS", timeMs);
    feedbackAttachment = std::make_unique< juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "DELAY_FEEDBACK", feedback);
    wetAttachment = std::make_unique< juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "DELAY_WET_LEVEL", wet);
    cutoffAttachment = std::make_unique< juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "DELAY_CUTOFF", cutoff);

    timeMs.setLabelNames("Time", "Time");
    feedback.setLabelNames("Feedback", "Feedback");
    wet.setLabelNames("Wet Level", "Wet");
    cutoff.setLabelNames("Cutoff Frequency", "Cutoff");

    timeMs.setModMatrix(modMatrix, "DELAY_TIME_MS");
    feedback.setModMatrix(modMatrix, "DELAY_FEEDBACK");
    wet.setModMatrix(modMatrix, "DELAY_WET_LEVEL");
    cutoff.setModMatrix(modMatrix, "DELAY_CUTOFF");

    cutoff.setSkewFactorFromMidPoint(1000.0f);
}

DelayDisplay::~DelayDisplay() {
    setLookAndFeel(nullptr);
}

void DelayDisplay::paint(juce::Graphics& g) {
    g.fillAll(GlowStyle::charcoal);
}

void DelayDisplay::resized() {
    auto area = getLocalBounds().reduced(10);

    int knobWidth = area.getWidth() / 6;
    timeMs.setBounds(area.removeFromLeft(knobWidth));
    feedback.setBounds(area.removeFromLeft(knobWidth));
    wet.setBounds(area.removeFromLeft(knobWidth));
    cutoff.setBounds(area.removeFromLeft(knobWidth));
    area.removeFromLeft(knobWidth);

    graph.setBounds(area.removeFromLeft(knobWidth));
}
