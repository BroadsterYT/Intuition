/*
  ==============================================================================

    IntumiManager.cpp
    Created: 4 Nov 2025 10:38:02pm
    Author:  BroDe

  ==============================================================================
*/

#include "IntumiManager.h"


IntumiManager::IntumiManager() {

}

IntumiManager::~IntumiManager() {
    auto convoFile = getSessionConvoFile();
    convoFile.deleteFile();
}

juce::File IntumiManager::getSessionConvoFile() {
    juce::File location = juce::File::getSpecialLocation(juce::File::tempDirectory);
    juce::String filePath = uuid.toString();
    filePath << ".json";
    juce::File convoFile = location.getChildFile(filePath);

    if (!convoFile.existsAsFile()) {
        convoFile.create();
        convoFile.replaceWithText("{\"conversationId\": \"default\", \"messages\": []}");
    }
    DBG("Session conversation file: " << convoFile.getFullPathName());
    return convoFile;
}

juce::String IntumiManager::queryAI(const juce::String& apiKey, const juce::String& prompt, const juce::String& params) {
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
