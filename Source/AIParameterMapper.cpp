/*
  ==============================================================================

    AIParameterMapper.cpp
    Created: 2025
    Author: Intuition + AI

  ==============================================================================
*/

#include "AIParameterMapper.h"

AIParameterMapper::AIParameterMapper(juce::AudioProcessorValueTreeState& params)
    : parameters(params)
{
}

AIParameterMapper::~AIParameterMapper()
{
}

bool AIParameterMapper::applyParameterChanges(const juce::var& aiResponse, juce::String& errorMessage)
{
    if (!aiResponse.isObject())
    {
        errorMessage = "Invalid AI response format: not an object";
        return false;
    }

    auto* responseObj = aiResponse.getDynamicObject();
    if (!responseObj || !responseObj->hasProperty("parameters"))
    {
        errorMessage = "AI response missing 'parameters' field";
        return false;
    }

    auto parametersArray = responseObj->getProperty("parameters");
    if (!parametersArray.isArray())
    {
        errorMessage = "AI response 'parameters' is not an array";
        return false;
    }

    int changesApplied = 0;
    juce::StringArray failedParams;

    // Apply each parameter change
    for (int i = 0; i < parametersArray.size(); ++i)
    {
        auto paramChange = parametersArray[i];
        if (!paramChange.isObject())
            continue;

        auto* changeObj = paramChange.getDynamicObject();
        if (!changeObj)
            continue;

        juce::String paramName = changeObj->getProperty("name").toString();
        float value = static_cast<float>(changeObj->getProperty("value"));
        bool isRelative = changeObj->getProperty("relative");

        // Get the parameter
        auto* param = parameters.getParameter(paramName);
        if (param == nullptr)
        {
            failedParams.add(paramName + " (not found)");
            continue;
        }

        // Get the parameter's range
        auto range = getParameterRange(paramName);

        // Clamp value to valid range
        value = juce::jlimit(range.start, range.end, value);

        // Convert to normalized value (0-1) for JUCE
        float normalizedValue = range.convertTo0to1(value);

        // If relative change, add to current normalized value
        if (isRelative)
        {
            float currentNormalized = param->getValue();
            normalizedValue = currentNormalized + (value / (range.end - range.start));
            normalizedValue = juce::jlimit(0.0f, 1.0f, normalizedValue);
        }

        // Apply the change
        param->setValueNotifyingHost(normalizedValue);
        changesApplied++;
    }

    if (changesApplied == 0)
    {
        errorMessage = "No parameters were changed. Failed: " + failedParams.joinIntoString(", ");
        return false;
    }

    if (failedParams.size() > 0)
    {
        errorMessage = "Applied " + juce::String(changesApplied) + " changes. Failed: " + failedParams.joinIntoString(", ");
        return true; // Partial success
    }

    errorMessage = "Successfully applied " + juce::String(changesApplied) + " parameter changes";
    return true;
}

juce::String AIParameterMapper::getSystemPrompt()
{
    return R"(You are an AI assistant that controls a wavetable synthesizer called Intuition.
Your job is to translate user requests into JSON parameter changes.

AVAILABLE PARAMETERS:
- MASTER (0.0-1.0): Master volume
- A_TOGGLE, B_TOGGLE, C_TOGGLE, D_TOGGLE (bool): Enable/disable oscillators A, B, C, D
- A_VOLUME, B_VOLUME, C_VOLUME, D_VOLUME (0.0-1.0): Oscillator volumes
- A_DETUNE, B_DETUNE, C_DETUNE, D_DETUNE (0-100): Oscillator detune amount
- A_MORPH, B_MORPH, C_MORPH, D_MORPH (0.0-1.0): Wavetable morph position
- A_OCTAVE, B_OCTAVE, C_OCTAVE, D_OCTAVE (-4 to 4): Octave shift
- A_COARSE, B_COARSE, C_COARSE, D_COARSE (-12 to 12): Semitone shift
- FILTER_CUTOFF (20.0-20000.0): Filter cutoff frequency in Hz
- FILTER_RESONANCE (0.01-1.0): Filter resonance
- REVERB_TOGGLE (bool): Enable/disable reverb
- REVERB_DAMPING (0.0-1.0): Reverb high frequency damping
- REVERB_ROOM_SIZE (0.0-1.0): Reverb room size
- REVERB_WIDTH (0.0-1.0): Reverb stereo width
- REVERB_DRY_LEVEL (0.0-1.0): Dry signal level
- REVERB_WET_LEVEL (0.0-1.0): Wet (reverb) signal level
- ENV_OSC_ATTACK, ENV_OSC_DECAY, ENV_OSC_SUSTAIN, ENV_OSC_RELEASE (0.0-1.0): Main envelope

INTERPRETATION GUIDE:
- "warm": Lower filter cutoff (500-2000Hz), slight detune (5-15), increase morph
- "bright": Higher filter cutoff (5000-12000Hz), reduce resonance
- "punchy": Fast attack (0.0-0.05), short decay (0.1-0.3), moderate sustain (0.5-0.7)
- "soft": Slower attack (0.2-0.5), longer release (0.5-0.8)
- "spacey": Enable reverb, large room size (0.7-1.0), high wet level (0.4-0.7)
- "detune": Increase detune (20-50)
- "fat": Enable multiple oscillators, add detune (10-30)
- "thin": Single oscillator, no detune
- "add bass": Enable oscillator at -1 or -2 octave
- "more treble": Increase filter cutoff

RESPONSE FORMAT (JSON only, no other text):
{
  "description": "Brief description of what you're doing",
  "parameters": [
    {"name": "PARAM_NAME", "value": 0.5, "relative": false},
    {"name": "ANOTHER_PARAM", "value": 0.1, "relative": true}
  ]
}

IMPORTANT RULES:
1. Always respond with valid JSON only
2. Use "relative": true for adjustments (e.g., "increase by"), false for absolute values
3. For normalized parameters (0.0-1.0), treat them as normalized (not 0-100)
4. Be conservative with changes - don't make dramatic shifts
5. If toggling oscillators/effects, remember to set their parameters too
6. Multiple small related changes are better than one big change

Example: "make it warm"
{
  "description": "Making the sound warmer by lowering filter cutoff and adding subtle detune",
  "parameters": [
    {"name": "FILTER_CUTOFF", "value": 1500.0, "relative": false},
    {"name": "A_DETUNE", "value": 10.0, "relative": false},
    {"name": "A_MORPH", "value": 0.3, "relative": false}
  ]
}

Now respond to user requests with parameter changes!)";
}

bool AIParameterMapper::validateParameter(const juce::String& paramName, float value) const
{
    auto* param = parameters.getParameter(paramName);
    if (param == nullptr)
        return false;

    auto range = getParameterRange(paramName);
    return value >= range.start && value <= range.end;
}

juce::NormalisableRange<float> AIParameterMapper::getParameterRange(const juce::String& paramName) const
{
    auto* param = parameters.getParameter(paramName);
    if (param == nullptr)
        return juce::NormalisableRange<float>(0.0f, 1.0f);

    // For most parameters, the internal range is 0-1 (normalized)
    // The actual range is defined in the AudioParameterFloat/Int constructors
    // We need to map based on parameter type

    // Check if it's a filter cutoff (special case - logarithmic scale)
    if (paramName == "FILTER_CUTOFF")
        return juce::NormalisableRange<float>(20.0f, 20000.0f, 0.0f, 0.3f);

    // Check if it's filter resonance
    if (paramName == "FILTER_RESONANCE")
        return juce::NormalisableRange<float>(0.01f, 1.0f);

    // Check if it's a detune parameter
    if (paramName.contains("DETUNE"))
        return juce::NormalisableRange<float>(0.0f, 100.0f);

    // Check if it's an octave parameter
    if (paramName.contains("OCTAVE"))
        return juce::NormalisableRange<float>(-4.0f, 4.0f);

    // Check if it's a coarse parameter
    if (paramName.contains("COARSE"))
        return juce::NormalisableRange<float>(-12.0f, 12.0f);

    // Check if it's a fine parameter
    if (paramName.contains("FINE"))
        return juce::NormalisableRange<float>(-100.0f, 100.0f);

    // Default normalized range
    return juce::NormalisableRange<float>(0.0f, 1.0f);
}

float AIParameterMapper::clampToRange(const juce::String& paramName, float value) const
{
    auto range = getParameterRange(paramName);
    return juce::jlimit(range.start, range.end, value);
}
