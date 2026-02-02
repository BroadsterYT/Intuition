/*
  ==============================================================================

    ChorusDisplay.h
    Created: 2 Feb 2026 1:06:52pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ModMatrix.h"
#include "ButtonComponent.h"
#include "SliderComponent.h"


class ChorusDisplay : public juce::Component {
public:
    ChorusDisplay(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;

    ButtonComponent toggle;

    SliderComponent rate, depth, width, dry, wet;
};
