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
    if (bandCoeffs.size() <= 0) {
        DBG("ZERO BAND COEFFS");
        return;
    }
    DBG("b0: " << bandCoeffs[0][0] << " b1: " << bandCoeffs[0][1] << " b2: " << bandCoeffs[0][2] << " a1: " << bandCoeffs[0][3] << " a2: " << bandCoeffs[0][4]);
    ItnLookAndFeel::drawEqualizer(g, getLocalBounds().toFloat(), bandCoeffs);
}

void EqualizerGraph::timerCallback() {
    repaint();
}
