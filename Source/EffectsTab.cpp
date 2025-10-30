/*
  ==============================================================================

    EffectsTab.cpp
    Created: 30 Oct 2025 5:26:32pm
    Author:  BroDe

  ==============================================================================
*/

#include "EffectsTab.h"

EffectsTab::EffectsTab(ItnContext& context
) : context(context),
    reverb(context.parameters, context.modMatrix) {
    setLookAndFeel(&lookAndFeel);

    addAndMakeVisible(reverb);
}

EffectsTab::~EffectsTab() {
    setLookAndFeel(nullptr);
}

void EffectsTab::paint(juce::Graphics& g) {
    g.fillAll(GlowStyle::shadow);
}

void EffectsTab::resized() {
    int padding = 15;

    reverb.setBounds(padding, padding, 600, 200);
}
