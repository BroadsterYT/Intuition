/*
  ==============================================================================

    ItnFileHelper.cpp
    Created: 27 Jul 2026 11:21:29am
    Author:  BroDe

  ==============================================================================
*/

#include "ItnFileHelper.h"


void ItnFileHelper::configureItnHomeDirectory() {
    juce::File home = getItnHomeDirectory();
    if (!home.exists()) {
        home.createDirectory();
    }

    juce::File logs = home.getChildFile("Logs");
    juce::File presets = home.getChildFile("Presets");
    juce::File waveforms = home.getChildFile("Waveforms");
    logs.createDirectory();
    presets.createDirectory();
    waveforms.createDirectory();
}

juce::File ItnFileHelper::getItnHomeDirectory() {
    juce::File docsDir(juce::File::getSpecialLocation(juce::File::userDocumentsDirectory));
    juce::File itnDir = docsDir.getChildFile("Intuition");
    return itnDir;
}

juce::File ItnFileHelper::getIntumiConvoFileDirectory() {
    auto home = ItnFileHelper::getItnHomeDirectory();
    return home.getChildFile("Logs").getChildFile("Intumi");
}
