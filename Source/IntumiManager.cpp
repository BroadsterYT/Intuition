/*
  ==============================================================================

    IntumiManager.cpp
    Created: 4 Nov 2025 10:38:02pm
    Author:  BroDe

  ==============================================================================
*/

#include "IntumiManager.h"

IntumiManager::IntumiManager() {}

juce::String IntumiManager::queryAI(
    const juce::String apiKey,
    const juce::String prompt,
    const juce::String params
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

juce::File IntumiManager::getConvoFileByUuid(juce::Uuid& convoId) {
    juce::File logDir = getConvoDirectory();
    juce::File convoFile = logDir.getChildFile(convoId.toString() + ".json");
    if (!convoFile.existsAsFile()) {
        DBG("WARN: File " << convoFile.getFullPathName() << " does not exist or is not a file. Creating now...");
        convoFile.create();
    }
    return convoFile;
}

juce::File IntumiManager::getConvoDirectory() {
    juce::File logDir = juce::File(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory))
        .getChildFile("Intuition")
        .getChildFile("Logs")
        .getChildFile("Intumi");

    if (!logDir.exists()) {
        DBG("ERROR: Directory " << logDir.getFullPathName() << " does not exist. Make sure the user Intuition directory was created and configured.");
        return juce::File();
    }
    return logDir;
}
