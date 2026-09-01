/*
  ==============================================================================

    IntumiManager.h
    Created: 4 Nov 2025 10:38:02pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "JsonHelper.h"


class IntumiManager {
public:
    IntumiManager();

    juce::String getPayload(const juce::String& prompt, const juce::String& params);

    juce::String getHeaders(const juce::String& apiKey);
    
    /// <summary>
    /// Sends a query through the Intumi microservice and returns the AI message
    /// result.
    /// </summary>
    /// <param name="apiKey">The API key to send with the request</param>
    /// <param name="prompt">The user input to send in the request</param>
    /// <param name="params">The current plugin parameters as a JSON string</param>
    /// <returns>(juce::String) The JSON response sent back from the microservice</returns>
    juce::String queryAI(
        const juce::String& apiKey,
        const juce::String& prompt,
        const juce::String& params
    );

    /// <summary>
    /// Returns the API key to use in Intumi queries.
    /// </summary>
    /// <returns>juce::String API key</returns>
    juce::String getApiKey();
    /// <summary>
    /// Sets the API key to use for all Intumi API calls.
    /// </summary>
    /// <param name="newApiKey">The key to use for all subsequent API calls</param>
    /// <returns>True if the new key was set successfully, false otherwise</returns>
    bool setApiKey(const juce::String& newApiKey);

    /// <summary>
    /// Creates a new conversation file with a unique UUID.
    /// </summary>
    /// <returns>juce::File The newly created file</returns>
    juce::File createNewConvoFile();
    /// <summary>
    /// Given a UUID, returns the conversation file with that ID.
    /// </summary>
    /// <param name="convoId">The UUID of the conversation to retrieve</param>
    /// <returns>juce::File containing the conversation with the given UUID</returns>
    juce::File getConvoFileByUuid(juce::Uuid& convoId);
    /// <summary>
    /// Retrieves all files in the conversation file directory.
    /// </summary>
    /// <returns>juce::Array containing all juce::File convo files</returns>
    juce::Array<juce::File> getAllConvoFiles();

private:
    /// <summary>
    /// Returns the file containing the Intumi API key.
    /// </summary>
    /// <returns>juce::File File containing the API key</returns>
    juce::File getApiKeyFile();

    /// <summary>
    /// Returns the directory holding the conversation files
    /// </summary>
    /// <returns>The juce::File directory holding the conversation files</returns>
    juce::File getConvoDirectory();
};
