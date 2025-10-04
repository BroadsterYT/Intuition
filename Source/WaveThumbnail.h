/*
  ==============================================================================

    WaveThumbnail.h
    Created: 4 Oct 2025 4:50:47pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "WavetableBank.h"


class WaveThumbnail : public juce::Component {
public:
    WaveThumbnail(int id, WavetableBank& bank);

    void mouseDown(const juce::MouseEvent& e) override;
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    WavetableBank& bank;
    int waveId = 0; // ID of the wave for this component to draw
};