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
#include "ItnLookAndFeel.h"

/// <summary>
/// A simple little picture of a waveform used to quickly display the wave's shape
/// </summary>
class WaveThumbnail : public juce::Component {
public:
    WaveThumbnail(int id, WavetableBank& bank);
    ~WaveThumbnail() override;

    void mouseDown(const juce::MouseEvent& e) override;
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    ItnLookAndFeel lookAndFeel;

    WavetableBank& bank;
    int waveId = 0; // ID of the wave for this component to draw

    juce::Array<float> waveform;
    void buildReducedWaveform();
};