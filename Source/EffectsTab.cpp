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
    reverb(context.parameters, context.modMatrix),
    delay(context.parameters, context.modMatrix) {
    setLookAndFeel(&lookAndFeel);

    addAndMakeVisible(reverb);
    addAndMakeVisible(delay);
}

EffectsTab::~EffectsTab() {
    setLookAndFeel(nullptr);
}

void EffectsTab::paint(juce::Graphics& g) {
    g.fillAll(GlowStyle::shadow);
}

void EffectsTab::resized() {
    int padding = 120;
    int width = getWidth() - 2 * padding;

    reverb.setBounds(padding, 15, width, 150);
    delay.setBounds(padding, 15 + reverb.getBottom(), width, 150);
}
