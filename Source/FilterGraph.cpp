/*
  ==============================================================================

    FilterGraph.cpp
    Created: 19 Oct 2025 3:37:10pm
    Author:  BroDe

  ==============================================================================
*/

#include "FilterGraph.h"

FilterGraph::FilterGraph(
    juce::AudioProcessorValueTreeState& vts
) : parameters(vts) {
    setLookAndFeel(&lookAndFeel);
    startTimerHz(30);
}

FilterGraph::~FilterGraph() {
    setLookAndFeel(nullptr);
}

void FilterGraph::paint(juce::Graphics& g) {
    g.fillAll(GlowStyle::roomDark);
    
    float cutoff = *parameters.getRawParameterValue("FILTER_CUTOFF");
    float resonance = *parameters.getRawParameterValue("FILTER_RESONANCE");
    int typeChoice = (int)*parameters.getRawParameterValue("FILTER_TYPE");
    
    lookAndFeel.drawFilter(g, juce::Rectangle<float>(getWidth(), getHeight()), cutoff, resonance, typeChoice);
}

void FilterGraph::timerCallback() {
    repaint();
}