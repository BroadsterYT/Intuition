/*
  ==============================================================================

    AIManager.cpp
    Created: 4 Nov 2025 10:38:02pm
    Author:  BroDe

  ==============================================================================
*/

#include "AIManager.h"

AIManager::AIManager() {

}

void AIManager::setKey(const juce::String newKey) {
    apiKey = newKey;
}

juce::String AIManager::queryAI(const juce::String apiKey, const juce::String prompt) {
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
    intumi.start(tempExe.getFullPathName() + " --api-key " + apiKey + " --prompt \"" + prompt + "\"");

    intumi.waitForProcessToFinish(-1);
    juce::String output = intumi.readAllProcessOutput();

    DBG("Intumi says: " << output);
    return output;
}
