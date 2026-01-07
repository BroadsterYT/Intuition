/*
  ==============================================================================

    IntumiTab.cpp
    Created: 6 Nov 2025 8:02:04pm
    Author:  BroDe

  ==============================================================================
*/

#include "IntumiTab.h"
#include "JsonHelper.h"


IntumiTab::IntumiTab(juce::AudioProcessor* ap) {
    setLookAndFeel(&ItnLookAndFeel::getInstance());
    processor = dynamic_cast<IntuitionAudioProcessor*>(ap);

    // ----- API Key Box ----- //
    juce::File keyFile = getSavedKeyFile();
    if (!keyFile.existsAsFile()) {
        apiKeyBox.setTextToShowWhenEmpty("Enter Groq API key...", MinimalStyle::warmHighlight);
    }
    else {
        apiKeyBox.setTextToShowWhenEmpty("API key set successfully.", juce::Colours::green);
    }
    apiKeyBox.onReturnKey = [this]() {
        setApiKey(apiKeyBox.getText());
        apiKeyBox.clear();
        apiKeyBox.setTextToShowWhenEmpty("API key set successfully.", juce::Colours::green);
    };
    
    // ----- Prompt Box ----- //
    promptBox.setTextToShowWhenEmpty("Ask Intumi...", MinimalStyle::warmHighlight);
    promptBox.onReturnKey = [this]() {
        outputBox.moveCaretToEnd();
        
        // Retrieving messages array
        juce::File convoFile(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("Intuition/Logs/Intumi/convo.json"));
        appendUserMessageToConversation(convoFile, "user", promptBox.getText(), processor->getParametersAsJsonString());
        
        // API query
        juce::String intumiResponse = AIManager::queryAI(
            getApiKey(),
            promptBox.getText(),
            processor->getParametersAsJsonString()
        );

        //appendUserMessageToConversation(convoFile, "intumi", intumiResponse);

        juce::var response = convertStringToJson(intumiResponse);
        juce::DynamicObject::Ptr obj = response.getDynamicObject();
        if (!obj) {
            outputBox.insertTextAtCaret(intumiResponse);  // Displays error
            return;
        }
        
        juce::String message = obj->getProperty("message");
        outputBox.insertTextAtCaret(message + "\n");
        juce::var jsonParams = obj->getProperty("parameters");
        processor->applyJsonParameterTweaks(jsonParams);
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
    g.fillAll(MinimalStyle::roomDark);
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

void IntumiTab::appendUserMessageToConversation(const juce::File& jsonFile, const juce::String& role, const juce::String& message, const juce::String& parametersJsonString) {
    juce::var jsonConvo = JsonHelper::getJsonFileAsVar(jsonFile);
    auto* root = jsonConvo.getDynamicObject();
    if (!root) {
        DBG("DynamicObject could not be retrieved from JSON file: " << jsonFile.getFullPathName());
        return;
    }

    juce::var messagesVar = root->getProperty("messages");
    if (!messagesVar.isArray()) {
        DBG("The value of \"mesaages\" is not of type array.");
        return;
    }
    auto* messages = messagesVar.getArray();

    // Formatting user content as json
    juce::var userContent(new juce::DynamicObject());
    auto* contentRoot = userContent.getDynamicObject();
    contentRoot->setProperty("parameters", convertStringToJson(parametersJsonString));
    contentRoot->setProperty("message", message);

    // Adding user query to messages json
    juce::var userMsg(new juce::DynamicObject());
    auto* userDyn = userMsg.getDynamicObject();
    userDyn->setProperty("id", juce::Uuid().toString());
    userDyn->setProperty("role", role);
    userDyn->setProperty("timestamp", (juce::int64)juce::Time::getCurrentTime().toMilliseconds());
    userDyn->setProperty("content", userContent);
    messages->add(userMsg);
    jsonFile.replaceWithText(juce::JSON::toString(jsonConvo));
}
