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
    const char* exeData;
    int exeSize;

#if defined(JUCE_WINDOWS) && JUCE_WINDOWS
    exeData = BinaryData::intumi_exe;
    exeSize = BinaryData::intumi_exeSize;
#elif defined(JUCE_MAC) && JUCE_MAC
    exeData = BinaryData::intumi;
    exeSize = BinaryData::intumiSize;
#endif

    juce::File tempExe = juce::File::getSpecialLocation(juce::File::tempDirectory).getChildFile("intumi.exe");
    tempExe.replaceWithData(exeData, exeSize);
    tempExe.setExecutePermission(true);
    DBG(tempExe.getFullPathName());

    juce::ChildProcess intumi;
    intumi.start(tempExe.getFullPathName() 
        + " --api-key " + apiKey 
        + " --prompt \"" + prompt + "\""
        + " --params \"" + params
    );

    intumi.waitForProcessToFinish(-1);
    juce::String output = intumi.readAllProcessOutput();

    //DBG("Intumi says: " << output);
    return output;
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
