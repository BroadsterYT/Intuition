/*
  ==============================================================================

    IntumiTab.cpp
    Created: 6 Nov 2025 8:02:04pm
    Author:  BroDe

  ==============================================================================
*/

#include "IntumiTab.h"


IntumiTab::IntumiTab(juce::AudioProcessor* ap) {
    setLookAndFeel(&lookAndFeel);

    processor = dynamic_cast<IntuitionAudioProcessor*>(ap);

    juce::File keyFile = getSavedKeyFile();
    if (!keyFile.existsAsFile()) {
        apiKeyBox.setTextToShowWhenEmpty("Enter Groq API key...", GlowStyle::warmHighlight);
    }
    else {
        apiKeyBox.setTextToShowWhenEmpty("API key set successfully.", juce::Colours::green);
    }
    
    apiKeyBox.onReturnKey = [this]() {
        setApiKey(apiKeyBox.getText());
        apiKeyBox.clear();
        apiKeyBox.setTextToShowWhenEmpty("API key set successfully.", juce::Colours::green);
    };
    
    promptBox.setTextToShowWhenEmpty("Ask Intumi...", GlowStyle::warmHighlight);
    promptBox.onReturnKey = [this]() {
        outputBox.moveCaretToEnd();

        juce::String intumiResponse = AIManager::queryAI(
            getApiKey(),
            promptBox.getText(),
            processor->getParametersAsJsonString()
        );

        juce::var response = convertStringToJson(intumiResponse);
        juce::DynamicObject::Ptr obj = response.getDynamicObject();
        if (!obj) {
            outputBox.insertTextAtCaret(intumiResponse);
            return;
        }

        juce::String message = obj->getProperty("message");
        outputBox.insertTextAtCaret(message);
    };

    outputBox.setReadOnly(true);
    outputBox.setMultiLine(true);

    addAndMakeVisible(apiKeyBox);
    addAndMakeVisible(promptBox);
    addAndMakeVisible(outputBox);
}

IntumiTab::~IntumiTab() {
    setLookAndFeel(nullptr);
}

void IntumiTab::paint(juce::Graphics& g) {
    g.fillAll(GlowStyle::roomDark);
}

void IntumiTab::resized() {
    auto area = getLocalBounds();

    apiKeyBox.setBounds(50, 50, 1000, 40);
    promptBox.setBounds(50, 100, 1000, 40);
    outputBox.setBounds(50, 250, 1000, 360);
}

juce::File IntumiTab::getSavedKeyFile() {
    juce::File docsDir = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory);
    juce::File projDocsDir = docsDir.getChildFile("Intuition");
    projDocsDir.createDirectory();

    juce::File keySaveFile = projDocsDir.getChildFile("key.env");
    keySaveFile.create();
    return keySaveFile;
}

void IntumiTab::setApiKey(const juce::String newKey) {
    juce::File keySaveFile = getSavedKeyFile();
    keySaveFile.replaceWithText(newKey);
    DBG("API key set as: " << newKey);
}

juce::String IntumiTab::getApiKey() {
    juce::File keySaveFile = getSavedKeyFile();
    juce::String key = keySaveFile.loadFileAsString();
    DBG("API key returned: " << key);
    return key;
}

juce::var IntumiTab::convertStringToJson(const juce::String jsonString) {
    juce::var parsedJson;

    juce::Result result = juce::JSON::parse(jsonString, parsedJson);
    jassert(result.wasOk());

    return parsedJson;
}
