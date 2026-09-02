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
#include "ConvoTextBubble.h"
#include "ConvoCanvas.h"


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
    ConvoCanvas convoDisplay;

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
    /// Append a message from the user or Intumi AI to a conversation file
    /// </summary>
    /// <param name="jsonFile">File to append to</param>
    /// <param name="role">Role of sender, either "user" or "intumi"</param>
    /// <param name="message">Message body sent</param>
    void appendMessageToConvoFile(const juce::File& jsonFile, const juce::String& role, const juce::String& message);
};