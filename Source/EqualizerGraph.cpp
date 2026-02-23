/*
  ==============================================================================

    EqualizerGraph.cpp
    Created: 21 Feb 2026 1:23:40pm
    Author:  BroDe

  ==============================================================================
*/

#include "EqualizerGraph.h"

EqualizerGraph::EqualizerGraph(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix, EqualizerModule& equalizer
) : parameters(vts), modMatrix(modMatrix), equalizer(equalizer) {
    setLookAndFeel(&ItnLookAndFeel::getInstance());
    startTimerHz(30);
}

EqualizerGraph::~EqualizerGraph() {
    setLookAndFeel(nullptr);
    stopTimer();
}

void EqualizerGraph::paint(juce::Graphics& g) {
    std::vector<std::vector<float>> bandCoeffs;
    equalizer.getBandCoefficients(bandCoeffs);
    jassert(bandCoeffs.size() > 0);
    ItnLookAndFeel::drawEqualizer(g, getLocalBounds().toFloat(), bandCoeffs);

    // Drawing band locations
    float freq1 = modMatrix->getModdedDest("EQBAND1_FREQUENCY");
    float gain1 = modMatrix->getModdedDest("EQBAND1_GAIN");
    float q1 = modMatrix->getModdedDest("EQBAND1_Q");
    ItnLookAndFeel::drawEqualizerPoint(g, getLocalBounds().toFloat(), freq1, gain1, q1);
}

void EqualizerGraph::timerCallback() {
    repaint();
}
