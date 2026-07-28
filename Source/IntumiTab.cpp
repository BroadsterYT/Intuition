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

    juce::Array<juce::File> allConvoFiles = intumi.getAllConvoFiles();
    if (!allConvoFiles.size()) {
        juce::File newConvoFile = intumi.createNewConvoFile();
    }
    juce::File testFile = allConvoFiles.getFirst();
    renderAllPreviousMessages(testFile);

    // ----- API Key Box ----- //
    if (intumi.getApiKey().isEmpty()) {
        apiKeyBox.setTextToShowWhenEmpty("Enter Groq API key...", MinimalStyle::accentOrange);
    }
    else {
        apiKeyBox.setTextToShowWhenEmpty("API key set successfully.", juce::Colours::green);
    }
    apiKeyBox.onReturnKey = [this]() {
        intumi.setApiKey(apiKeyBox.getText());
        apiKeyBox.clear();
        apiKeyBox.setTextToShowWhenEmpty("API key set successfully.", juce::Colours::green);
    };
    
    // ----- Prompt Box ----- //
    promptBox.setTextToShowWhenEmpty("Ask Intumi...", MinimalStyle::accentOrange);
    promptBox.onReturnKey = [this]() {
        // Retrieving messages array
        juce::File convoFile = intumi.getAllConvoFiles().getFirst();
        appendUserMessageToConversation(convoFile, promptBox.getText(), processor->getParametersAsJsonString());
        convoDisplay.addMessage("user", promptBox.getText());

        // API query
        juce::String intumiResponse = intumi.queryAI(
            intumi.getApiKey(),
            promptBox.getText(),
            processor->getParametersAsJsonString()
        );
        appendIntumiMessageToConversation(convoFile, intumiResponse);

        juce::var response = JsonHelper::getJsonStringAsVar(intumiResponse);
        juce::DynamicObject::Ptr obj = response.getDynamicObject();
        if (!obj) {
            // TODO: Find way to display errors with updated Intumi UI
            return;
        }
        
        juce::String message = obj->getProperty("message");
        convoDisplay.addMessage("intumi", message);

        juce::var jsonParams = obj->getProperty("parameters");
        processor->applyJsonParameterTweaks(jsonParams);
    };

    convoViewport.setViewedComponent(&convoDisplay);
    convoViewport.setScrollBarsShown(true, false);

    addAndMakeVisible(apiKeyBox);
    addAndMakeVisible(promptBox);
    addAndMakeVisible(convoViewport);
}

IntumiTab::~IntumiTab() {
    setLookAndFeel(nullptr);
}

void IntumiTab::paint(juce::Graphics& g) {
    g.fillAll(MinimalStyle::bgDark);
}

void IntumiTab::resized() {
    auto area = getLocalBounds();
    apiKeyBox.setBounds(50, 50, 1000, 40);
    promptBox.setBounds(50, 100, 1000, 40);
    convoViewport.setBounds(50, 250, 1000, 360);
}

void IntumiTab::renderAllPreviousMessages(const juce::File& jsonFile) {
    juce::var jsonConvo = JsonHelper::getJsonFileAsVar(jsonFile);
    juce::var messagesVar = getConversationArray(jsonConvo);
    auto* messages = messagesVar.getArray();
    if (!messages) {
        DBG("ERROR: Could not retrieve messages array from " << jsonFile.getFullPathName());
        return;
    }

    for (int i = 0; i < messages->size(); ++i) {
        juce::var msgVar = messages->getUnchecked(i);
        
        auto* obj = msgVar.getDynamicObject();
        if (!obj) continue;

        juce::String role = obj->getProperty("role");
        juce::var contentVar = obj->getProperty("content");
        auto* contentObj = contentVar.getDynamicObject();
        if (!contentObj) {
            DBG("ERROR: Could not get DynamicObject from message content.");
            continue;
        }

        juce::String msgText = contentObj->getProperty("message");
        DBG("Message: " << msgText);

        convoDisplay.addMessage(role, msgText, true);
    }
}

juce::var IntumiTab::getConversationArray(juce::var& jsonVar) {
    auto* root = jsonVar.getDynamicObject();
    if (!root) {
        DBG("ERROR: DynamicObject could not be retrieved from JSON file.");
        return juce::var();
    }

    juce::var messagesVar = root->getProperty("messages");
    if (!messagesVar.isArray()) {
        DBG("ERROR: The value of \"messages\" is not of type array.");
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
