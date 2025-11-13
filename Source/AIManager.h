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
    
    static juce::String queryAI(
        const juce::String apiKey,
        const juce::String prompt,
        const juce::String params
    );
};
