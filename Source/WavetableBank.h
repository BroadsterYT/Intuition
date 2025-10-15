/*
  ==============================================================================

    WavetableBank.h
    Created: 3 Oct 2025 5:48:09pm
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/// <summary>
/// A class with functions for formatting/manipulating wavetables
/// </summary>
class WavetableBank {
public:
    WavetableBank();

    /// <summary>
    /// Adds a new wavetable to the wavebank
    /// </summary>
    /// <param name="newTable">The wavetable to add</param>
    void addWavetable(const juce::AudioBuffer<float>& newTable);
    /// <summary>
    /// Removes a wavetable from the wavebank. If there is only 1 wavetable in the
    /// bank, it will not be removed.
    /// </summary>
    /// <param name="index">The index of the wavetable to remove</param>
    void removeWavetable(int index);
    /// <summary>
    /// Updates the wavetable at the given index to match the given wavetable
    /// </summary>
    /// <param name="index">The index of the wavetable to update</param>
    /// <param name="table">The wavetable data to update with</param>
    void updateWavetable(int index, const juce::AudioBuffer<float>& table);
    
    /// <summary>
    /// Returns the wavetable at the given index
    /// </summary>
    /// <param name="index">The index of the wavetable to retrieve</param>
    /// <returns>The wavetable at the given index</returns>
    juce::AudioBuffer<float>& getWavetable(int index);
    
    size_t size();

private:
    std::vector<juce::AudioBuffer<float>> wavetables;
};