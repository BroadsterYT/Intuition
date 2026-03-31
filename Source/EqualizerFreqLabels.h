/*
  ==============================================================================

    EqualizerFreqLabels.h
    Created: 29 Mar 2026 4:35:57pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnLookAndFeel.h"


class EqualizerFreqLabels : public juce::Component {
public:
    EqualizerFreqLabels();
    ~EqualizerFreqLabels();

    void paint(juce::Graphics& g) override;

private:
    void drawRangeBox(juce::Graphics& g, const float posY, float freqMin, float freqMax, const juce::String& text, const bool noSpace = false, const float xSpace = 2.0f, juce::Justification just = juce::Justification::centred);
};