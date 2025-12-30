/*
  ==============================================================================

    ReverbDisplay.cpp
    Created: 30 Oct 2025 6:37:22pm
    Author:  BroDe

  ==============================================================================
*/

#include "ReverbDisplay.h"

ReverbDisplay::ReverbDisplay(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix
) : parameters(vts), modMatrix(modMatrix), graph(vts, modMatrix),
    damping(vts, "REVERB_DAMPING", "DAMPING", "REVERB_DAMPING"),
    roomSize(vts, "REVERB_ROOM_SIZE", "ROOM SIZE", "REVERB_ROOM_SIZE"),
    rvbWidth(vts, "REVERB_WIDTH", "WIDTH", "REVERB_WIDTH"),
    dryLevel(vts, "REVERB_DRY_LEVEL", "DRY", "REVERB_DRY_LEVEL"),
    wetLevel(vts, "REVERB_WET_LEVEL", "WET", "REVERB_WET_LEVEL") {
    setLookAndFeel(&ItnLookAndFeel::getInstance());

    addAndMakeVisible(toggle);
    addAndMakeVisible(damping);
    addAndMakeVisible(roomSize);
    addAndMakeVisible(rvbWidth);
    addAndMakeVisible(dryLevel);
    addAndMakeVisible(wetLevel);
    addAndMakeVisible(graph);

    toggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(parameters, "REVERB_TOGGLE", toggle);

    damping.setModMatrix(modMatrix);
    roomSize.setModMatrix(modMatrix);
    rvbWidth.setModMatrix(modMatrix);
    dryLevel.setModMatrix(modMatrix);
    wetLevel.setModMatrix(modMatrix);
}

ReverbDisplay::~ReverbDisplay() {
    setLookAndFeel(nullptr);
}

void ReverbDisplay::paint(juce::Graphics& g) {
    g.fillAll(MinimalStyle::charcoal);
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
