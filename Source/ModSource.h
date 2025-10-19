/*
  ==============================================================================

    ModSource.h
    Created: 15 Oct 2025 11:23:24pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class ModSource {
public:
    ModSource(const juce::String name, float* value = nullptr);

    juce::String getName() const;
    float getValue() const;
    void setValuePtr(float* newVal);

private:
    juce::String name;
    float* value = nullptr;
};