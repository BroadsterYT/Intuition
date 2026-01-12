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

    juce::File convoFile(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("Intuition/Logs/Intumi/convo.json"));
    if (!convoFile.existsAsFile()) {
        convoFile.create();
        convoFile.replaceWithText("{\"conversationId\": \"default\", \"messages\": []}");
    }
    renderAllPreviousMessages(convoFile);

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
        if (!convoFile.existsAsFile()) {
            convoFile.create();
            convoFile.replaceWithText("{\"conversationId\": \"default\", \"messages\": []}");
        }
        appendUserMessageToConversation(convoFile, promptBox.getText(), processor->getParametersAsJsonString());
        convoView.addMessage("user", promptBox.getText());

        // API query
        juce::String intumiResponse = AIManager::queryAI(
            getApiKey(),
            promptBox.getText(),
            processor->getParametersAsJsonString()
        );
        appendIntumiMessageToConversation(convoFile, intumiResponse);

        juce::var response = JsonHelper::getJsonStringAsVar(intumiResponse);
        juce::DynamicObject::Ptr obj = response.getDynamicObject();
        if (!obj) {
            outputBox.insertTextAtCaret(intumiResponse);  // Displays error
            return;
        }
        
        juce::String message = obj->getProperty("message");
        convoView.addMessage("intumi", message);

        juce::var jsonParams = obj->getProperty("parameters");
        processor->applyJsonParameterTweaks(jsonParams);
    };

    outputBox.setReadOnly(true);
    outputBox.setMultiLine(true);

    convoViewport.setViewedComponent(&convoView);
    convoViewport.setScrollBarsShown(true, false);

    addAndMakeVisible(apiKeyBox);
    addAndMakeVisible(promptBox);
    addAndMakeVisible(convoViewport);
    //addAndMakeVisible(outputBox);
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
    //outputBox.setBounds(50, 250, 1000, 360);

    convoViewport.setBounds(50, 250, 1000, 360);
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

void IntumiTab::renderAllPreviousMessages(const juce::File& jsonFile) {
    juce::var jsonConvo = JsonHelper::getJsonFileAsVar(jsonFile);
    juce::var messagesVar = getConversationArray(jsonConvo);
    auto* messages = messagesVar.getArray();

    for (int i = 0; i < messages->size(); ++i) {
        juce::var msgVar = messages->getUnchecked(i);
        
        auto* obj = msgVar.getDynamicObject();
        if (!obj) continue;

        juce::String role = obj->getProperty("role");
        juce::var contentVar = obj->getProperty("content");
        auto* contentObj = contentVar.getDynamicObject();
        if (!contentObj) {
            DBG("Error: Could not get DynamicObject from message content.");
            continue;
        }

        juce::String msgText = contentObj->getProperty("message");
        DBG("Message: " << msgText);

        convoView.addMessage(role, msgText, true);
    }
}

juce::var IntumiTab::getConversationArray(juce::var& jsonVar) {
    auto* root = jsonVar.getDynamicObject();
    if (!root) {
        DBG("Error: DynamicObject could not be retrieved from JSON file.");
        return juce::var();
    }

    juce::var messagesVar = root->getProperty("messages");
    if (!messagesVar.isArray()) {
        DBG("Error: The value of \"messages\" is not of type array.");
        return juce::var();
    }
    return messagesVar;
}

void IntumiTab::appendUserMessageToConversation(const juce::File& jsonFile, const juce::String& message, const juce::String& parametersJsonString) {
    juce::var jsonConvo = JsonHelper::getJsonFileAsVar(jsonFile);
    juce::var messagesVar = getConversationArray(jsonConvo);
    auto* messages = messagesVar.getArray();

    // Formatting user content as json
    juce::var userContent(new juce::DynamicObject());
    auto* contentRoot = userContent.getDynamicObject();
    contentRoot->setProperty("parameters", JsonHelper::getJsonStringAsVar(parametersJsonString));
    contentRoot->setProperty("message", message);

    // Adding user query to messages json
    juce::var userMsg(new juce::DynamicObject());
    auto* userDyn = userMsg.getDynamicObject();
    userDyn->setProperty("id", juce::Uuid().toString());
    userDyn->setProperty("role", "user");
    userDyn->setProperty("timestamp", (juce::int64)juce::Time::getCurrentTime().toMilliseconds());
    userDyn->setProperty("content", userContent);
    messages->add(userMsg);
    jsonFile.replaceWithText(juce::JSON::toString(jsonConvo));
}

void IntumiTab::appendIntumiMessageToConversation(const juce::File& jsonFile, const juce::String& jsonResponse) {
    juce::var jsonConvo = JsonHelper::getJsonFileAsVar(jsonFile);
    juce::var messagesVar = getConversationArray(jsonConvo);
    auto* messages = messagesVar.getArray();

    juce::var intumiContent = JsonHelper::getJsonStringAsVar(jsonResponse);

    juce::var finalMsg(new juce::DynamicObject());
    auto* finalDyn = finalMsg.getDynamicObject();
    finalDyn->setProperty("id", juce::Uuid().toString());
    finalDyn->setProperty("role", "intumi");
    finalDyn->setProperty("timestamp", (juce::int64)juce::Time::getCurrentTime().toMilliseconds());
    finalDyn->setProperty("content", intumiContent);
    messages->add(finalMsg);
    jsonFile.replaceWithText(juce::JSON::toString(jsonConvo));
}
