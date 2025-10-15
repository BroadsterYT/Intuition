/*
  ==============================================================================

    WavetableHelper.h
    Created: 15 Oct 2025 1:10:30am
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class WavetableHelper {
public:
    static void preprocessWavetable(juce::AudioBuffer<float>& wavetable);
    static void phaseAlignWavetable(juce::AudioBuffer<float>& wavetable);
};