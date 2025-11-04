/*
  ==============================================================================

    AIParameterMapper.h
    Created: 2025
    Author: Intuition + AI

    Maps AI responses to synth parameters with validation and smoothing.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AIParameterMapper
{
public:
    AIParameterMapper(juce::AudioProcessorValueTreeState& params);
    ~AIParameterMapper();

    // Apply parameter changes from AI response JSON
    // Expected format: { "parameters": [ {"name": "PARAM_NAME", "value": 0.5, "relative": false}, ... ] }
    bool applyParameterChanges(const juce::var& aiResponse, juce::String& errorMessage);

    // Get the system prompt that teaches the AI about available parameters
    static juce::String getSystemPrompt();

    // Validate a parameter change before applying
    bool validateParameter(const juce::String& paramName, float value) const;

private:
    juce::AudioProcessorValueTreeState& parameters;

    // Helper to get parameter range
    juce::NormalisableRange<float> getParameterRange(const juce::String& paramName) const;

    // Helper to clamp value to valid range
    float clampToRange(const juce::String& paramName, float value) const;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AIParameterMapper)
};
