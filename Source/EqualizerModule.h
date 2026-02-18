/*
  ==============================================================================

    EqualizerModule.h
    Created: 18 Feb 2026 4:14:56pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "EQBand.h"


class EqualizerModule {
public:
    EqualizerModule();

    void prepare(double sr, int samplesPerBlock, int numChannels);
    void processBlock(juce::AudioBuffer<float>& buffer);

private:
    std::vector<EQBand> bands;
    double sampleRate = 44100.0;
};