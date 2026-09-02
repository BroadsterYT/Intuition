/*
  ==============================================================================

    IntumiManager.cpp
    Created: 4 Nov 2025 10:38:02pm
    Author:  BroDe

  ==============================================================================
*/

#include "IntumiManager.h"
#include "ItnFileHelper.h"


IntumiManager::IntumiManager() {}

juce::String IntumiManager::queryAI(
    const juce::String& apiKey,
    const juce::String& prompt,
    const juce::String& params
) {
    juce::URL endpoint("https://api.groq.com/openai/v1/chat/completions");
    juce::String payload = getPayload(prompt, params);

    endpoint = endpoint.withPOSTData(payload);

    int statusCode = 0;
    auto options = juce::URL::InputStreamOptions(juce::URL::ParameterHandling::inPostData)
        .withHttpRequestCmd("POST")
        .withStatusCode(&statusCode)
        .withExtraHeaders("Authorization: Bearer " + apiKey + "\r\nContent-Type: application/json");

    auto stream = endpoint.createInputStream(options);
    if (!stream) {
        juce::String errorMsg = "I'm having trouble thinking... "
                                "try connecting to the internet, dummy!! >~~<";
        juce::String output = formatStringAsIntumiResponse(errorMsg);
        DBG("Output error: " << output);
        return output;
    }
    
    juce::String response = stream->readEntireStreamAsString();
    if (statusCode != 200) {
        juce::String errorMsg = getErrorFromResponse(response);
        juce::String formattedErrorMsg = "Oh, no! An error "
            + juce::String(statusCode)
            + L" occurred! (\u2565\uFE4F\u2565)"
            + "\n(" + errorMsg + ")";
        juce::String output = formatStringAsIntumiResponse(formattedErrorMsg);
        return output;
    }

    juce::String intumiMsg = getIntumiResponseContent(response);
    return intumiMsg;
}

juce::String IntumiManager::getApiKey() {
    juce::File keyFile = getApiKeyFile();
    juce::String apiKey = keyFile.loadFileAsString();
    if (apiKey.isEmpty()) {
        DBG("WARN: API key retrieved from key file is empty.");
    }
    return apiKey;
}

bool IntumiManager::setApiKey(const juce::String& newApiKey) {
    juce::File keyFile = getApiKeyFile();
    return keyFile.replaceWithText(newApiKey);
}

juce::File IntumiManager::createNewConvoFile() {
    juce::Uuid newId;
    juce::File convoDir = getConvoDirectory();
    juce::File newConvoFile = convoDir.getChildFile(newId.toString() + ".json");
    if (newConvoFile.create().wasOk()) {
        DBG("New conversation file was created with UUID " << newId.toString());
        newConvoFile.replaceWithText("{\"messages\": []}");
    }
    return newConvoFile;
}

juce::File IntumiManager::getConvoFileByUuid(juce::Uuid& convoId) {
    juce::File logDir = getConvoDirectory();
    juce::File convoFile = logDir.getChildFile(convoId.toString() + ".json");
    if (!convoFile.existsAsFile()) {
        DBG("WARN: File " << convoFile.getFullPathName() << " does not exist or is not a file.");
        return juce::File();
    }
    return convoFile;
}

juce::Array<juce::File> IntumiManager::getAllConvoFiles() {
    juce::File convoDir = getConvoDirectory();
    juce::Array<juce::File> childFiles = convoDir.findChildFiles(juce::File::findFiles, false, "*");
    return childFiles;
}

juce::String IntumiManager::getPayload(const juce::String& prompt, const juce::String& params) {
    juce::var payload(new juce::DynamicObject());
    juce::DynamicObject::Ptr payloadDynObj = payload.getDynamicObject();

    payloadDynObj->setProperty("model", "openai/gpt-oss-120b");
    payloadDynObj->setProperty("reasoning_format", "hidden");

    // Defining system behavior
    juce::var systemObj(new juce::DynamicObject());
    juce::DynamicObject::Ptr systemDynObj = systemObj.getDynamicObject();
    systemDynObj->setProperty("role", "system");
    juce::String systemRole = juce::String::fromUTF8(
        BinaryData::system_role_dat,
        BinaryData::system_role_datSize);
    systemDynObj->setProperty("content", systemRole);

    // User input
    juce::var userObj(new juce::DynamicObject());
    juce::DynamicObject::Ptr userDynObj = userObj.getDynamicObject();
    userDynObj->setProperty("role", "user");
    userDynObj->setProperty("content", prompt + params);

    juce::Array<juce::var> messagesArray;
    messagesArray.add(systemObj);
    messagesArray.add(userObj);

    payloadDynObj->setProperty("messages", messagesArray);

    juce::String jsonOutput = juce::JSON::toString(payload);
    return jsonOutput;
}

juce::String IntumiManager::getHeaders(const juce::String& apiKey) {
    juce::var headers(new juce::DynamicObject());
    juce::DynamicObject::Ptr headersDynObj = headers.getDynamicObject();

    headersDynObj->setProperty("Authorization", apiKey);
    headersDynObj->setProperty("Content-Type", "application/json");

    juce::String jsonOutput = juce::JSON::toString(headers);
    return jsonOutput;
}

juce::String IntumiManager::getIntumiResponseContent(const juce::String& response) {
    juce::var responseJson = JsonHelper::getJsonStringAsVar(response);
    juce::DynamicObject::Ptr jsonObj = responseJson.getDynamicObject();
    
    jassert(jsonObj->getProperty("choices").isArray());
    juce::Array<juce::var> choicesArray = *jsonObj->getProperty("choices").getArray();
    juce::String intumiContent = choicesArray[0]
        .getDynamicObject()->getProperty("message")
        .getDynamicObject()->getProperty("content");
    return intumiContent;
}

juce::String IntumiManager::formatStringAsIntumiResponse(const juce::String& value) {
    juce::var json(new juce::DynamicObject());
    juce::DynamicObject::Ptr jsonObj = json.getDynamicObject();
    jsonObj->setProperty("parameters", juce::var(new juce::DynamicObject()));
    jsonObj->setProperty("message", value);
    return juce::JSON::toString(json);
}

juce::String IntumiManager::getErrorFromResponse(const juce::String& response) {
    juce::var responseJson = JsonHelper::getJsonStringAsVar(response);
    juce::DynamicObject::Ptr jsonObj = responseJson.getDynamicObject();
    juce::DynamicObject::Ptr errorObj = jsonObj->getProperty("error").getDynamicObject();
    juce::String errorMsg = errorObj->getProperty("message");
    return errorMsg;
}

juce::File IntumiManager::getApiKeyFile() {
    juce::File homeDir = ItnFileHelper::getItnHomeDirectory();
    juce::File keyFile = homeDir.getChildFile("key.env");
    if (!keyFile.existsAsFile()) {
        DBG("ERROR: key.env file does not exist. Creating file...");
        keyFile.create();
    }
    return keyFile;
}

juce::File IntumiManager::getConvoDirectory() {
    juce::File homeDir = ItnFileHelper::getItnHomeDirectory();
    juce::File logDir = homeDir.getChildFile("Logs").getChildFile("Intumi");
    if (!logDir.exists()) {
        DBG("ERROR: Directory " << logDir.getFullPathName() << " does not exist. Make sure the user Intuition directory was created and configured.");
        return juce::File();
    }
    return logDir;
}
