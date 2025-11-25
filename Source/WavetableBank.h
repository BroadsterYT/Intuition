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
/// A struct used to store all information regarding a waveform. Necessary for saving
/// wavetable information when saving/loading the plugin
/// </summary>
struct WavetableInfo {
    juce::File filePath;
    bool isNative = false;
    char* localData;
};

/// <summary>
/// A class with functions for formatting/manipulating wavetables
/// </summary>
class WavetableBank {
public:
    WavetableBank();

    /// <summary>
    /// Adds a wavetable from a .wav file to the wavebank
    /// </summary>
    /// <param name="wavFile">The wavtable .wav</param>
    void addWavetable(juce::File& wavFile);
    void addWavetable(const void* binaryWav, int binarySize);
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