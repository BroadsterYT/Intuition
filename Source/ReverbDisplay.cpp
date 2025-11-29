/*
  ==============================================================================

    ReverbDisplay.cpp
    Created: 30 Oct 2025 6:37:22pm
    Author:  BroDe

  ==============================================================================
*/

#include "ReverbDisplay.h"

ReverbDisplay::ReverbDisplay(
    juce::AudioProcessorValueTreeState& vts,
    ModMatrix* modMatrix
) : parameters(vts),
    modMatrix(modMatrix),
    graph(vts, modMatrix) {
    setLookAndFeel(&ItnLookAndFeel::getInstance());

    addAndMakeVisible(toggle);
    addAndMakeVisible(damping);
    addAndMakeVisible(roomSize);
    addAndMakeVisible(rvbWidth);
    addAndMakeVisible(dryLevel);
    addAndMakeVisible(wetLevel);
    addAndMakeVisible(graph);

    toggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(parameters, "REVERB_TOGGLE", toggle);
    dampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "REVERB_DAMPING", damping);
    roomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "REVERB_ROOM_SIZE", roomSize);
    rvbWidthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "REVERB_WIDTH", rvbWidth);
    dryLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "REVERB_DRY_LEVEL", dryLevel);
    wetLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "REVERB_WET_LEVEL", wetLevel);

    damping.setLabelName("Damp");
    roomSize.setLabelName("Size");
    rvbWidth.setLabelName("Width");
    dryLevel.setLabelName("Dry");
    wetLevel.setLabelName("Wet");

    damping.setModMatrix(modMatrix, "REVERB_DAMPING");
    roomSize.setModMatrix(modMatrix, "REVERB_ROOM_SIZE");
    rvbWidth.setModMatrix(modMatrix, "REVERB_WIDTH");
    dryLevel.setModMatrix(modMatrix, "REVERB_DRY_LEVEL");
    wetLevel.setModMatrix(modMatrix, "REVERB_WET_LEVEL");

    damping.setCustomTooltipText("REVERB_DAMPING");
    roomSize.setCustomTooltipText("REVERB_ROOM_SIZE");
    rvbWidth.setCustomTooltipText("REVERB_WIDTH");
    dryLevel.setCustomTooltipText("REVERB_DRY_LEVEL");
    wetLevel.setCustomTooltipText("REVERB_WET_LEVEL");
}

ReverbDisplay::~ReverbDisplay() {
    setLookAndFeel(nullptr);
}

void ReverbDisplay::paint(juce::Graphics& g) {
    g.fillAll(GlowStyle::charcoal);
}

void ReverbDisplay::resized() {
    auto area = getLocalBounds().reduced(10);

    toggle.setBounds(area.removeFromLeft(25));
    int knobWidth = area.getWidth() / 6;
    damping.setBounds(area.removeFromLeft(knobWidth));
    roomSize.setBounds(area.removeFromLeft(knobWidth));
    rvbWidth.setBounds(area.removeFromLeft(knobWidth));
    dryLevel.setBounds(area.removeFromLeft(knobWidth));
    wetLevel.setBounds(area.removeFromLeft(knobWidth));

    graph.setBounds(area.removeFromLeft(knobWidth));
}
