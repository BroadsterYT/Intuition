/*
  ==============================================================================

    MainTabs.cpp
    Created: 23 Oct 2025 8:28:34pm
    Author:  BroDe

  ==============================================================================
*/

#include "MainTabs.h"


MainTabs::MainTabs(ItnContext& context
) : tabs(juce::TabbedButtonBar::TabsAtTop),
    context(context),
    oscTab(context),
    fxTab(context) {
    startTimerHz(60);

    addAndMakeVisible(tabs);
    tabs.addTab("Osc", juce::Colours::darkgrey, &oscTab, false);
    tabs.addTab("FX", juce::Colours::darkgrey, &fxTab, false);
    tabs.addTab("Intumi", juce::Colours::darkgrey, &intumiTab, false);
}

void MainTabs::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
}

void MainTabs::resized() {
    tabs.setBounds(getLocalBounds());
}

void MainTabs::timerCallback() {
    repaint();
}
