/*
  ==============================================================================

    JsonHelper.cpp
    Created: 7 Jan 2026 12:06:37am
    Author:  BroDe

  ==============================================================================
*/

#include "JsonHelper.h"

juce::var JsonHelper::getJsonFileAsVar(const juce::File& jsonFile) {
    if (!jsonFile.existsAsFile()) {
        DBG("JSON file not found " << jsonFile.getFullPathName());
        return juce::var();
    }

    juce::String jsonString = jsonFile.loadFileAsString();
    if (jsonString.isEmpty()) {
        DBG("JSON file is empty: " << jsonFile.getFullPathName());
        return juce::var();
    }

    juce::var jsonVar = juce::JSON::parse(jsonString);
    if (jsonVar.isVoid()) {
        DBG("Failed to parse JSON: " << jsonFile.getFullPathName());
        return juce::var();
    }

    return jsonVar;
}
