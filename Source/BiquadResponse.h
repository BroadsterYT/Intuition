/*
  ==============================================================================

    BiquadResponse.h
    Created: 20 Feb 2026 8:42:05pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class BiquadResponse {
public:
    static float magnitudeToDecibels(float mag);
    static float getDecibelsAtFrequency(float freq, float sampleRate, float b0, float b1, float b2, float a1, float a2);
};
