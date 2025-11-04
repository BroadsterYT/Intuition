/*
  ==============================================================================

    GeminiAPIClient.h
    Created: 2025
    Author: Intuition + AI

    This class handles communication with Google AI Studio (Gemini API)
    for AI-powered parameter control.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <functional>

class GeminiAPIClient : public juce::Thread
{
public:
    GeminiAPIClient();
    ~GeminiAPIClient() override;

    // Callback types
    using SuccessCallback = std::function<void(const juce::var&)>;
    using ErrorCallback = std::function<void(const juce::String&)>;

    // Send a prompt to Gemini API
    void sendPrompt(
        const juce::String& apiKey,
        const juce::String& systemPrompt,
        const juce::String& userPrompt,
        SuccessCallback onSuccess,
        ErrorCallback onError
    );

    // Cancel any ongoing request
    void cancelRequest();

    // Check if a request is in progress
    bool isRequestInProgress() const { return requestInProgress; }

private:
    void run() override;

    // Request data
    juce::String currentApiKey;
    juce::String currentSystemPrompt;
    juce::String currentUserPrompt;
    SuccessCallback currentSuccessCallback;
    ErrorCallback currentErrorCallback;

    std::atomic<bool> requestInProgress{ false };
    std::atomic<bool> shouldCancel{ false };

    juce::CriticalSection callbackLock;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GeminiAPIClient)
};
