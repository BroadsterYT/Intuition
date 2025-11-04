/*
  ==============================================================================

    AIControlPanel.h
    Created: 2025
    Author: Intuition + AI

    UI component for AI-powered parameter control.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GeminiAPIClient.h"
#include "AIParameterMapper.h"

class AIControlPanel : public juce::Component,
                       public juce::TextEditor::Listener,
                       public juce::Button::Listener
{
public:
    AIControlPanel(juce::AudioProcessorValueTreeState& params);
    ~AIControlPanel() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    // Set/get API key
    void setApiKey(const juce::String& key);
    juce::String getApiKey() const;

    // TextEditor::Listener
    void textEditorReturnKeyPressed(juce::TextEditor&) override;

    // Button::Listener
    void buttonClicked(juce::Button* button) override;

private:
    void sendAIRequest();
    void handleAISuccess(const juce::var& response);
    void handleAIError(const juce::String& error);
    void updateStatus(const juce::String& message, juce::Colour colour);

    juce::AudioProcessorValueTreeState& parameters;

    // UI Components
    juce::Label titleLabel;
    juce::Label apiKeyLabel;
    juce::TextEditor apiKeyEditor;
    juce::TextButton saveApiKeyButton;

    juce::Label promptLabel;
    juce::TextEditor promptEditor;
    juce::TextButton sendButton;

    juce::Label statusLabel;
    juce::Label lastResponseLabel;

    // Backend
    std::unique_ptr<GeminiAPIClient> apiClient;
    std::unique_ptr<AIParameterMapper> paramMapper;

    juce::String currentApiKey;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AIControlPanel)
};
