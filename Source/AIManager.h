/*
  ==============================================================================

    AIManager.h
    Created: 4 Nov 2025 10:38:02pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class AIManager {
public:
    AIManager();

    /// <summary>
    /// Sets the API key to use when requesting a response
    /// </summary>
    /// <param name="newKey">String containing API key to use</param>
    void setKey(const juce::String newKey);
    
    static juce::String queryAI(const juce::String apiKey, const juce::String prompt);

private:
    juce::String apiKey = "";
};
