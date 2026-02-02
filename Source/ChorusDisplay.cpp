/*
  ==============================================================================

    ChorusDisplay.cpp
    Created: 2 Feb 2026 1:06:52pm
    Author:  BroDe

  ==============================================================================
*/

#include "ChorusDisplay.h"

ChorusDisplay::ChorusDisplay(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix
) : parameters(vts), modMatrix(modMatrix), toggle(vts, "CHORUS_TOGGLE"),
    rate(vts, "CHORUS_RATE", "RATE", "CHORUS_RATE", "Hz"),
    depth(vts, "CHORUS_DEPTH", "DEPTH", "CHORUS_DEPTH", "ms"),
    width(vts, "CHORUS_WIDTH", "WIDTH", "CHORUS_WIDTH"),
    dry(vts, "CHORUS_DRY_LEVEL", "DRY", "CHORUS_DRY_LEVEL"),
    wet(vts, "CHORUS_WET_LEVEL", "WET", "CHORUS_WET_LEVEL") {

    addAndMakeVisible(toggle);
    addAndMakeVisible(rate);
    addAndMakeVisible(depth);
    addAndMakeVisible(width);
    addAndMakeVisible(dry);
    addAndMakeVisible(wet);

    rate.setModMatrix(modMatrix);
    depth.setModMatrix(modMatrix);
    width.setModMatrix(modMatrix);
    dry.setModMatrix(modMatrix);
    wet.setModMatrix(modMatrix);
}

void ChorusDisplay::paint(juce::Graphics& g) {
}

void ChorusDisplay::resized() {
    auto area = getLocalBounds().reduced(10);
    toggle.setBounds(area.removeFromLeft(25));

    int knobWidth = area.getWidth() / 6;
    rate.setBounds(area.removeFromLeft(knobWidth));
    depth.setBounds(area.removeFromLeft(knobWidth));
    width.setBounds(area.removeFromLeft(knobWidth));
    dry.setBounds(area.removeFromLeft(knobWidth));
    wet.setBounds(area.removeFromLeft(knobWidth));
}
