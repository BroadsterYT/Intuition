/*
  ==============================================================================

    LFOTabs.cpp
    Created: 4 Oct 2025 5:05:46pm
    Author:  BroDe

  ==============================================================================
*/

#include "LFOTabs.h"


LFOTabs::LFOTabs(
    LFOShape& shape1,
    LFOShape& shape2,
    LFOShape& shape3
) : tabs(juce::TabbedButtonBar::TabsAtTop),
    shape1(shape1),
    shape2(shape2),
    shape3(shape3), 
    editor1(shape1), 
    editor2(shape2),
    editor3(shape3) {

    tabs.addTab("LFO 1", juce::Colours::darkgrey, &editor1, false);
    tabs.addTab("LFO 2", juce::Colours::darkgrey, &editor2, false);
    tabs.addTab("LFO 3", juce::Colours::darkgrey, &editor3, false);
    
    addAndMakeVisible(tabs);
}

void LFOTabs::resized() {
    tabs.setBounds(getLocalBounds());
}