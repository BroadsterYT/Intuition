/*
  ==============================================================================

    IntumiTab.h
    Created: 6 Nov 2025 8:02:04pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnLookAndFeel.h"
#include "AIManager.h"



class IntumiTab : public juce::Component {
public:
    IntumiTab();
    ~IntumiTab() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    ItnLookAndFeel lookAndFeel;

    juce::TextEditor apiKeyBox;
    juce::TextEditor promptBox;
    juce::TextEditor outputBox;

    /// <summary>
    /// Retrieves the file where the API key is saved. If no key has been saved, the
    /// file will be created and returned blank.
    /// </summary>
    /// <returns>The file with the saved API key</returns>
    juce::File getSavedKeyFile();
    /// <summary>
    /// Sets a new API key as the referenced key for Intuition.
    /// </summary>
    /// <param name="newKey">The API key for Intuition to reference</param>
    void setApiKey(const juce::String newKey);
    /// <summary>
    /// Retrieves the API key (if given before) and returns it
    /// </summary>
    /// <returns>An API key (if given one before)</returns>
    juce::String getApiKey();
};