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

    /// <summary>
    /// Returns the binary .WAV data given its corresponding ID
    /// </summary>
    /// <param name="id">The ID of the .WAV data to return</param>
    /// <returns>An std::pair containing the binary data of the .WAV and its size</returns>
    static std::pair<const char*, int> getWavBinary(int id);
    /// <summary>
    /// Given the binary data of a .WAV, returns its corresponding ID.
    /// </summary>
    /// <param name="binary">The binary data to get the ID of</param>
    /// <returns>The ID of the binary .WAV. If an invalid binary stream is given, will return -1.</returns>
    static int getWavId(const char* binary);
};