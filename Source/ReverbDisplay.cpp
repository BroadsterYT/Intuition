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
    modMatrix(modMatrix) {
    setLookAndFeel(&lookAndFeel);

    addAndMakeVisible(damping);
    addAndMakeVisible(roomSize);
    addAndMakeVisible(rvbWidth);
    addAndMakeVisible(dryLevel);
    addAndMakeVisible(wetLevel);

    dampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "REVERB_DAMPING", damping);
    roomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "REVERB_ROOM_SIZE", roomSize);
    rvbWidthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "REVERB_WIDTH", rvbWidth);
    dryLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "REVERB_DRY_LEVEL", dryLevel);
    wetLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "REVERB_WET_LEVEL", wetLevel);

    damping.setLabelNames("Damping", "Damp");
    roomSize.setLabelNames("Room Size", "Size");
    rvbWidth.setLabelNames("Width", "Width");
    dryLevel.setLabelNames("Dry Level", "Dry");
    wetLevel.setLabelNames("Wet Level", "Wet");

    damping.setModMatrix(modMatrix, "REVERB_DAMPING");
    roomSize.setModMatrix(modMatrix, "REVERB_ROOM_SIZE");
    rvbWidth.setModMatrix(modMatrix, "REVERB_WIDTH");
    dryLevel.setModMatrix(modMatrix, "REVERB_DRY_LEVEL");
    wetLevel.setModMatrix(modMatrix, "REVERB_WET_LEVEL");
}

ReverbDisplay::~ReverbDisplay() {
    setLookAndFeel(nullptr);
}

void ReverbDisplay::paint(juce::Graphics& g) {
    g.fillAll(GlowStyle::charcoal);
}

void ReverbDisplay::resized() {
    auto area = getLocalBounds().reduced(10);

    int knobWidth = area.getWidth() / 5;
    damping.setBounds(area.removeFromLeft(knobWidth));
    roomSize.setBounds(area.removeFromLeft(knobWidth));
    rvbWidth.setBounds(area.removeFromLeft(knobWidth));
    dryLevel.setBounds(area.removeFromLeft(knobWidth));
    wetLevel.setBounds(area.removeFromLeft(knobWidth));
}
