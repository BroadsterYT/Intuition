/*
  ==============================================================================

    EqualizerDisplay.cpp
    Created: 21 Feb 2026 1:23:26pm
    Author:  BroDe

  ==============================================================================
*/

#include "EqualizerDisplay.h"

EqualizerDisplay::EqualizerDisplay(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix, EqualizerModule& equalizer) : parameters(vts), modMatrix(modMatrix), graph(vts, modMatrix, equalizer) {
    setLookAndFeel(&ItnLookAndFeel::getInstance());
    addAndMakeVisible(graph);
    addAndMakeVisible(freqLabels);
}

EqualizerDisplay::~EqualizerDisplay() {
    setLookAndFeel(nullptr);
}

void EqualizerDisplay::paint(juce::Graphics& g) {
    g.fillAll(MinimalStyle::bgPanel);
}

void EqualizerDisplay::resized() {
    auto area = getLocalBounds();
    auto freqLabelArea = area.removeFromTop(26);

    freqLabels.setBounds(freqLabelArea);
    graph.setBounds(area);
}
