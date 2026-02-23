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

    // Visual
    
    /// <summary>
    /// Coverts a value from a frequency (logarithmic) scale into a point on a linear range
    /// </summary>
    /// <param name="freq">The frequency to covert</param>
    /// <param name="width">The width of the linear range. For drawing purposes, this would be the width of the component being drawn onto.</param>
    /// <param name="minFreq">The minimum frequency of the frequency range. Defaults to 20 Hz.</param>
    /// <param name="maxFreq">The maximum frequency of the frequency range. Defaults to 20 kHz.</param>
    /// <returns>The frequency point in the linear scale</returns>
    static float getFreqInLinearRange(float freq, float width, float minFreq = 20.0f, float maxFreq = 20000.0f);
};
