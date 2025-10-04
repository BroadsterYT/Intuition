/*
  ==============================================================================

    WavetableBank.cpp
    Created: 3 Oct 2025 5:48:18pm
    Author:  Brody

  ==============================================================================
*/

#include "WavetableBank.h"


WavetableBank::WavetableBank() {

}

void WavetableBank::addWavetable(const juce::AudioBuffer<float>& newTable) {
    wavetables.push_back(newTable);
}

void WavetableBank::removeWavetable(juce::AudioBuffer<float>* table) {
    if (!table) return;
    for (int i = 0; i < wavetables.size(); ++i) {
        if (table == &wavetables[i]) {
            wavetables.erase(wavetables.begin() + i);
            return;
        }
    }
}

void WavetableBank::updateWavetable(int index, const juce::AudioBuffer<float>& table) {
    wavetables[index].makeCopyOf(table);
}

juce::AudioBuffer<float>& WavetableBank::getWavetable(int index) {
    return wavetables[index];
}

void WavetableBank::clear() {
    wavetables.clear();
}

size_t WavetableBank::size() {
    return wavetables.size();
}