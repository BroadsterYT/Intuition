/*
  ==============================================================================

    WavetableBank.h
    Created: 3 Oct 2025 5:48:09pm
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class WavetableBank {
public:
    WavetableBank();

    void addWavetable(const juce::AudioBuffer<float>& newTable);
    void removeWavetable(juce::AudioBuffer<float>* table);
    void updateWavetable(int index, const juce::AudioBuffer<float>& table);
    
    juce::AudioBuffer<float>& getWavetable(int index);
    
    void clear();
    size_t size();

private:
    std::vector<juce::AudioBuffer<float>> wavetables;
};