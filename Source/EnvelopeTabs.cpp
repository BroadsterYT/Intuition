/*
  ==============================================================================

    EnvelopeTabs.cpp
    Created: 24 Oct 2025 3:56:42pm
    Author:  BroDe

  ==============================================================================
*/

#include "EnvelopeTabs.h"

EnvelopeTabs::EnvelopeTabs(juce::AudioProcessorValueTreeState& vts
) : parameters(vts),
    tabs(juce::TabbedButtonBar::TabsAtTop),
    envOsc(
        vts,
        "ENV_OSC_ATTACK",
        "ENV_OSC_DECAY",
        "ENV_OSC_SUSTAIN",
        "ENV_OSC_RELEASE"
    ),
    env1(
        vts,
        "ENV1_ATTACK",
        "ENV1_DECAY",
        "ENV1_SUSTAIN",
        "ENV1_RELEASE"
    ),
    env2(
        vts,
        "ENV2_ATTACK",
        "ENV2_DECAY",
        "ENV2_SUSTAIN",
        "ENV2_RELEASE"
    ), 
    env3(
        vts,
        "ENV3_ATTACK",
        "ENV3_DECAY",
        "ENV3_SUSTAIN",
        "ENV3_RELEASE"
    ) {
    addAndMakeVisible(tabs);

    tabs.addTab("Env Osc", juce::Colours::burlywood, &envOsc, false);
    tabs.addTab("Env 1", juce::Colours::darkgrey, &env1, false);
    tabs.addTab("Env 2", juce::Colours::darkgrey, &env2, false);
    tabs.addTab("Env 3", juce::Colours::darkgrey, &env3, false);
}

void EnvelopeTabs::resized() {
    tabs.setBounds(getLocalBounds());
}
