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
    for (int i = 0; i < 8; ++i) {
        auto& band = equalizer.getBand(i);
        std::vector<float> coeffs;
        band.getBiquadCoefficients(coeffs);
        bandCoeffs.push_back(coeffs);
    }
    jassert(bandCoeffs.size() > 0);
    ItnLookAndFeel::drawEqualizer(g, getLocalBounds().toFloat(), bandCoeffs);

    // Drawing band locations
    for (int i = 0; i < 8; ++i) {
        ItnLookAndFeel::drawEqualizerPoint(g, getLocalBounds().toFloat(), equalizer.getBand(i));
    }
}

void EqualizerGraph::timerCallback() {
    repaint();
}
