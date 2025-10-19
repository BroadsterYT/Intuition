/*
  ==============================================================================

    ModDestination.h
    Created: 15 Oct 2025 11:23:42pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"


/// <summary>
/// Holds a parameter index as reference for any modulations applied.
/// </summary>
class ModDestination {
public:
    ModDestination(const juce::String name, std::atomic<float>* baseValue = nullptr);

    juce::String getName();
    /// <summary>
    /// Returns the minimum of the range of the parameter this
    /// destination represents.
    /// </summary>
    /// <returns>Minimum of the range</returns>
    float getMinRange() const;
    void setMinRange(float min);
    /// <summary>
    /// Returns the maximum of the range of the parameter this
    /// destination represents.
    /// </summary>
    /// <returns>Maximum of the range</returns>
    float getMaxRange() const;
    void setMaxRange(float max);

    float getBaseValue();
    void setBasePtr(std::atomic<float>* base);
    /// <summary>
    /// Returns the modified value of this destination, after all
    /// mods have been applied
    /// </summary>
    /// <returns>Modded destination value</returns>
    float getModdedValue();
    void setModdedValue(float newVal);

private:
    juce::String name;
    float minRange = 0.0f;
    float maxRange = 1.0f;

    std::atomic<float>* baseValue = nullptr;
    float moddedValue = 0.0f;
};