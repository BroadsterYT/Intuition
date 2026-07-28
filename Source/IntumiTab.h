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
#include "IntumiManager.h"
#include "PluginProcessor.h"
#include "ConvoMessageComponent.h"
#include "ConvoDisplay.h"


class IntumiTab : public juce::Component {
public:
    IntumiTab(juce::AudioProcessor* ap);
    ~IntumiTab() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    IntuitionAudioProcessor* processor = nullptr;
    IntumiManager intumi;

    juce::TextEditor apiKeyBox;
    juce::TextEditor promptBox;

    juce::Viewport convoViewport;
    ConvoDisplay convoDisplay;

    // ----- Conversation ----- //

    /// <summary>
    /// Given a JSON conversation file, will render all messages into the conversation window
    /// </summary>
    /// <param name="jsonFile">A JSON file containing an AI conversation log</param>
    void renderAllPreviousMessages(const juce::File& jsonFile);

    /// <summary>
    /// Given a juce::var of an entire conversation JSON, returns a juce::var containing an array of all the messages sent and received.
    /// </summary>
    /// <param name="jsonVar">A juce::var containing a JSON conversation</param>
    /// <returns>A juce::var containing an array of all the messages sent and received in a specific conversation</returns>
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