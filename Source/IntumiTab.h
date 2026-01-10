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
#include "PluginProcessor.h"
#include "ConvoMessageComponent.h"
#include "ConvoView.h"


class IntumiTab : public juce::Component {
public:
    IntumiTab(juce::AudioProcessor* ap);
    ~IntumiTab() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    IntuitionAudioProcessor* processor = nullptr;

    juce::TextEditor apiKeyBox;
    juce::TextEditor promptBox;
    juce::TextEditor outputBox;

    juce::Viewport convoViewport;
    ConvoView convoView;

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

    juce::var getConversationArray(juce::var& jsonVar);
    /// <summary>
    /// Adds a user's message to the json file containing the current Intumi conversation after formatting it.
    /// </summary>
    /// <param name="jsonFile">The file containing the conversation</param>
    /// <param name="role">The role of the sender of the message</param>
    /// <param name="message">The prompt the user netered into the prompt box</param>
    /// <param name="parametersJsonString">A string containing all current parameter values</param>
    void appendUserMessageToConversation(const juce::File& jsonFile, const juce::String& message, const juce::String& parametersJsonString);
    void appendIntumiMessageToConversation(const juce::File& jsonFile, const juce::String& jsonResponse);
};