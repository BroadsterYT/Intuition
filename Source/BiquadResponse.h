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
    /// <summary>
    /// Finds the frequency for the nth octave of a note, relative to A4 (440Hz)
    /// </summary>
    /// <param name="n">The nth octave</param>
    /// <param name="offset">The semitone offset relative to A4. (an offset of 9 yields frequencies for C)</param>
    /// <returns>The frequency of the note at octave n</returns>
    static float getNthOctaveFreq(int n, int offset);

    // Visual
    
    /// <summary>
    /// Converts a value from a frequency (logarithmic) scale into a point on a linear range
    /// </summary>
    /// <param name="freq">The frequency to covert</param>
    /// <param name="width">The width of the linear range. For drawing purposes, this would be the width of the component being drawn onto.</param>
    /// <param name="minFreq">The minimum frequency of the frequency range. Defaults to 20 Hz.</param>
    /// <param name="maxFreq">The maximum frequency of the frequency range. Defaults to 20 kHz.</param>
    /// <returns>The frequency point in the linear scale</returns>
    static float getFreqInLinearRange(float freq, float width, float minFreq = 20.0f, float maxFreq = 20000.0f);
    /// <summary>
    /// Converts a linear value into its equivalent value on a logarithmic scale
    /// </summary>
    /// <param name="val">The linear value to convert</param>
    /// <param name="width">The range of the linear space</param>
    /// <param name="minFreq">The minimum frequency of the frequency range. Defaults to 20.0Hz</param>
    /// <param name="maxFreq">The maximum frequency of the frequency range. Defaults to 20000.0Hz</param>
    /// <returns>The linear value at the same position on a logarithmic scale</returns>
    static float getLinearValueAsFreq(float val, float width, float minFreq = 20.0f, float maxFreq = 20000.0f);
};
