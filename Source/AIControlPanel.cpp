/*
  ==============================================================================

    AIControlPanel.cpp
    Created: 2025
    Author: Intuition + AI

  ==============================================================================
*/

#include "AIControlPanel.h"

AIControlPanel::AIControlPanel(juce::AudioProcessorValueTreeState& params)
    : parameters(params)
{
    // Initialize backend
    apiClient = std::make_unique<GeminiAPIClient>();
    paramMapper = std::make_unique<AIParameterMapper>(parameters);

    // Title
    addAndMakeVisible(titleLabel);
    titleLabel.setText("AI Sound Control", juce::dontSendNotification);
    titleLabel.setFont(juce::Font(20.0f, juce::Font::bold));
    titleLabel.setJustificationType(juce::Justification::centred);

    // API Key section
    addAndMakeVisible(apiKeyLabel);
    apiKeyLabel.setText("Google AI Studio API Key:", juce::dontSendNotification);
    apiKeyLabel.setJustificationType(juce::Justification::left);

    addAndMakeVisible(apiKeyEditor);
    apiKeyEditor.setMultiLine(false);
    apiKeyEditor.setReturnKeyStartsNewLine(false);
    apiKeyEditor.setPasswordCharacter('*');
    apiKeyEditor.setTextToShowWhenEmpty("Enter your API key here...", juce::Colours::grey);

    addAndMakeVisible(saveApiKeyButton);
    saveApiKeyButton.setButtonText("Save Key");
    saveApiKeyButton.addListener(this);

    // Prompt section
    addAndMakeVisible(promptLabel);
    promptLabel.setText("What would you like to change?", juce::dontSendNotification);
    promptLabel.setJustificationType(juce::Justification::left);

    addAndMakeVisible(promptEditor);
    promptEditor.setMultiLine(true);
    promptEditor.setReturnKeyStartsNewLine(false);
    promptEditor.setTextToShowWhenEmpty("e.g., \"make it warm\", \"add brightness\", \"make it punchy\"...", juce::Colours::grey);
    promptEditor.addListener(this);

    addAndMakeVisible(sendButton);
    sendButton.setButtonText("Apply AI Changes");
    sendButton.addListener(this);

    // Status section
    addAndMakeVisible(statusLabel);
    statusLabel.setText("Ready", juce::dontSendNotification);
    statusLabel.setJustificationType(juce::Justification::centred);
    statusLabel.setColour(juce::Label::textColourId, juce::Colours::green);

    addAndMakeVisible(lastResponseLabel);
    lastResponseLabel.setText("", juce::dontSendNotification);
    lastResponseLabel.setJustificationType(juce::Justification::left);
    lastResponseLabel.setColour(juce::Label::textColourId, juce::Colours::lightblue);
    lastResponseLabel.setFont(juce::Font(12.0f, juce::Font::italic));
}

AIControlPanel::~AIControlPanel()
{
    promptEditor.removeListener(this);
    sendButton.removeListener(this);
    saveApiKeyButton.removeListener(this);
}

void AIControlPanel::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff1a1a1a));

    // Draw border
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 2);

    // Draw section dividers
    g.setColour(juce::Colours::darkgrey);
    int dividerY1 = 120;
    int dividerY2 = 260;
    g.drawLine(10, dividerY1, getWidth() - 10, dividerY1, 1.0f);
    g.drawLine(10, dividerY2, getWidth() - 10, dividerY2, 1.0f);
}

void AIControlPanel::resized()
{
    auto bounds = getLocalBounds().reduced(15);

    // Title
    titleLabel.setBounds(bounds.removeFromTop(30));
    bounds.removeFromTop(10);

    // API Key section
    apiKeyLabel.setBounds(bounds.removeFromTop(20));
    bounds.removeFromTop(5);

    auto apiKeyRow = bounds.removeFromTop(30);
    saveApiKeyButton.setBounds(apiKeyRow.removeFromRight(100));
    apiKeyRow.removeFromRight(10);
    apiKeyEditor.setBounds(apiKeyRow);
    bounds.removeFromTop(15);

    // Prompt section
    promptLabel.setBounds(bounds.removeFromTop(20));
    bounds.removeFromTop(5);
    promptEditor.setBounds(bounds.removeFromTop(80));
    bounds.removeFromTop(10);

    sendButton.setBounds(bounds.removeFromTop(40).reduced(50, 0));
    bounds.removeFromTop(15);

    // Status section
    statusLabel.setBounds(bounds.removeFromTop(25));
    bounds.removeFromTop(5);
    lastResponseLabel.setBounds(bounds.removeFromTop(60));
}

void AIControlPanel::setApiKey(const juce::String& key)
{
    currentApiKey = key;
    apiKeyEditor.setText(key, false);
}

juce::String AIControlPanel::getApiKey() const
{
    return currentApiKey;
}

void AIControlPanel::textEditorReturnKeyPressed(juce::TextEditor& editor)
{
    if (&editor == &promptEditor)
    {
        sendAIRequest();
    }
}

void AIControlPanel::buttonClicked(juce::Button* button)
{
    if (button == &sendButton)
    {
        sendAIRequest();
    }
    else if (button == &saveApiKeyButton)
    {
        currentApiKey = apiKeyEditor.getText();
        updateStatus("API key saved", juce::Colours::green);
    }
}

void AIControlPanel::sendAIRequest()
{
    juce::String prompt = promptEditor.getText().trim();

    if (prompt.isEmpty())
    {
        updateStatus("Please enter a command", juce::Colours::orange);
        return;
    }

    if (currentApiKey.isEmpty())
    {
        updateStatus("Please enter and save your API key first", juce::Colours::red);
        return;
    }

    if (apiClient->isRequestInProgress())
    {
        updateStatus("Request already in progress...", juce::Colours::orange);
        return;
    }

    updateStatus("Sending request to AI...", juce::Colours::yellow);
    sendButton.setEnabled(false);

    // Get the system prompt
    juce::String systemPrompt = AIParameterMapper::getSystemPrompt();

    // Send the request
    apiClient->sendPrompt(
        currentApiKey,
        systemPrompt,
        prompt,
        [this](const juce::var& response) { handleAISuccess(response); },
        [this](const juce::String& error) { handleAIError(error); }
    );
}

void AIControlPanel::handleAISuccess(const juce::var& response)
{
    sendButton.setEnabled(true);

    juce::String errorMessage;
    bool success = paramMapper->applyParameterChanges(response, errorMessage);

    if (success)
    {
        // Extract description if available
        juce::String description;
        if (response.isObject())
        {
            auto* obj = response.getDynamicObject();
            if (obj && obj->hasProperty("description"))
            {
                description = obj->getProperty("description").toString();
            }
        }

        updateStatus("Success!", juce::Colours::green);

        if (description.isNotEmpty())
        {
            lastResponseLabel.setText("AI: " + description, juce::dontSendNotification);
        }
        else
        {
            lastResponseLabel.setText(errorMessage, juce::dontSendNotification);
        }
    }
    else
    {
        updateStatus("Error applying changes", juce::Colours::red);
        lastResponseLabel.setText(errorMessage, juce::dontSendNotification);
    }
}

void AIControlPanel::handleAIError(const juce::String& error)
{
    sendButton.setEnabled(true);
    updateStatus("Request failed", juce::Colours::red);
    lastResponseLabel.setText("Error: " + error, juce::dontSendNotification);
}

void AIControlPanel::updateStatus(const juce::String& message, juce::Colour colour)
{
    statusLabel.setText(message, juce::dontSendNotification);
    statusLabel.setColour(juce::Label::textColourId, colour);
}
