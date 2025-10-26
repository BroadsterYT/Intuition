/*
  ==============================================================================

    EnvelopeGraph.h
    Created: 24 Oct 2025 1:26:21pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnContext.h"


/// <summary>
/// A component displaying an ADSR envelope
/// </summary>
class EnvelopeGraph : public juce::Component {
public:
    EnvelopeGraph(
        juce::AudioProcessorValueTreeState& vts,
        const GlobalEnvelope& env,
        const juce::String attackParamName,
        const juce::String decayParamName,
        const juce::String sustainParamName,
        const juce::String releaseParamName
    );

    void paint(juce::Graphics& g) override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    const GlobalEnvelope& env;

    juce::String attackParamName;
    juce::String decayParamName;
    juce::String sustainParamName;
    juce::String releaseParamName;

    void drawSecondMarkTicks(juce::Graphics& g);
    void drawEnvelopePath(juce::Graphics& g);
};