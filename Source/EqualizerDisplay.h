/*
  ==============================================================================

    EqualizerDisplay.h
    Created: 21 Feb 2026 1:23:26pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnLookAndFeel.h"
#include "ModMatrix.h"
#include "EqualizerGraph.h"
#include "EqualizerModule.h"


class EqualizerDisplay : public juce::Component {
public:
    EqualizerDisplay(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix, EqualizerModule& equalizer);
    ~EqualizerDisplay();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;

    EqualizerGraph graph;
};
