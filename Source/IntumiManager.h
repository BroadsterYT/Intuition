/*
  ==============================================================================

    IntumiManager.h
    Created: 4 Nov 2025 10:38:02pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class IntumiManager {
public:
    IntumiManager();
    ~IntumiManager();

    /// <summary>
    /// Returns the file being used for this session's Intumi conversation
    /// </summary>
    /// <returns>the juce::File holding this session's conversation</returns>
    juce::File getSessionConvoFile();

private:
    juce::Uuid uuid;

    juce::String queryAI(const juce::String& apiKey, const juce::String& prompt, const juce::String& params);
};
