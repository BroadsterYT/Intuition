/*
  ==============================================================================

    JsonHelper.h
    Created: 7 Jan 2026 12:06:37am
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class JsonHelper {
public:
    static juce::var getJsonFileAsVar(const juce::File& jsonFile);
    static juce::var getJsonStringAsVar(const juce::String& jsonString);
};
