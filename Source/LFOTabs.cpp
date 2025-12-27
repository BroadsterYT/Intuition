/*
  ==============================================================================

    LFOTabs.cpp
    Created: 4 Oct 2025 5:05:46pm
    Author:  BroDe

  ==============================================================================
*/

#include "LFOTabs.h"


LFOTabs::LFOTabs(
    juce::AudioProcessorValueTreeState& vts,
    LFOShape& shape1,
    float* phase1,
    LFOShape& shape2,
    float* phase2,
    LFOShape& shape3,
    float* phase3
) : parameters(vts),
    tabs(juce::TabbedButtonBar::TabsAtTop),
    shape1(shape1),
    shape2(shape2),
    shape3(shape3), 
    editor1(vts, shape1, phase1, "LFO1_MODE", "LFO1_RATE", "LFO1_SYNC_DIV"),
    editor2(vts, shape2, phase2, "LFO2_MODE", "LFO2_RATE", "LFO2_SYNC_DIV"),
    editor3(vts, shape3, phase3, "LFO3_MODE", "LFO3_RATE", "LFO3_SYNC_DIV") {

    tabs.addTab("LFO 1", juce::Colours::transparentBlack, &editor1, false);
    tabs.addTab("LFO 2", juce::Colours::transparentBlack, &editor2, false);
    tabs.addTab("LFO 3", juce::Colours::transparentBlack, &editor3, false);
    addAndMakeVisible(tabs);
}

void LFOTabs::resized() {
    tabs.setBounds(getLocalBounds());
}