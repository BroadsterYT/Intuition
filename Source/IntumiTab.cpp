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
        appendMessageToConvoFile(convoFile, "user", promptBox.getText());
        convoDisplay.addMessage("user", promptBox.getText());

        // API query
        juce::String intumiResponse = intumi.queryAI(
            intumi.getApiKey(),
            promptBox.getText(),
            processor->getParametersAsJsonString()
        );
        juce::var response = JsonHelper::getJsonStringAsVar(intumiResponse);
        juce::DynamicObject::Ptr obj = response.getDynamicObject();
        if (!obj) {
            return;
        }
        juce::String message = obj->getProperty("message");
        appendMessageToConvoFile(convoFile, "intumi", message);
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
        juce::String msgText = obj->getProperty("message");
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

void IntumiTab::appendMessageToConvoFile(const juce::File& jsonFile,
    const juce::String& role, const juce::String& message) {
    juce::var jsonConvo = JsonHelper::getJsonFileAsVar(jsonFile);
    juce::var messagesVar = getConversationArray(jsonConvo);
    auto* msgArray = messagesVar.getArray();

    // Adding formatted message to JSON file
    juce::var jsonMsg(new juce::DynamicObject());
    auto* msgDyn = jsonMsg.getDynamicObject();
    msgDyn->setProperty("id", juce::Uuid().toString());
    msgDyn->setProperty("role", role);
    msgDyn->setProperty("timestamp", (juce::int64)juce::Time::getCurrentTime().toMilliseconds());
    msgDyn->setProperty("message", message);
    msgArray->add(jsonMsg);
    jsonFile.replaceWithText(juce::JSON::toString(jsonConvo));
}