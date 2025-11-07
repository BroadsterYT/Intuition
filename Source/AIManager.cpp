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
    auto exeData = BinaryData::ai_micro_exe;
    auto exeSize = BinaryData::ai_micro_exeSize;

    juce::File tempExe = juce::File::getSpecialLocation(juce::File::tempDirectory).getChildFile("ai_comms.exe");

    tempExe.replaceWithData(exeData, exeSize);
    tempExe.setExecutePermission(true);

    DBG(tempExe.getFullPathName());

    juce::ChildProcess groq;
    groq.start(tempExe.getFullPathName() + " --api-key " + apiKey + " --prompt \"" + prompt + "\"");

    groq.waitForProcessToFinish(-1);
    juce::String output = groq.readAllProcessOutput();

    DBG("Intumi says: " << output);
    return output;
}
