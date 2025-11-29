/*
  ==============================================================================

    EnvelopeGraph.cpp
    Created: 24 Oct 2025 1:26:21pm
    Author:  BroDe

  ==============================================================================
*/

#include "EnvelopeGraph.h"

EnvelopeGraph::EnvelopeGraph(
    juce::AudioProcessorValueTreeState& vts,
    const GlobalEnvelope& env,
    const juce::String attackParamName,
    const juce::String decayParamName,
    const juce::String sustainParamName,
    const juce::String releaseParamName
) : parameters(vts),
    env(env),
    attackParamName(attackParamName),
    decayParamName(decayParamName),
    sustainParamName(sustainParamName),
    releaseParamName(releaseParamName) {
    setLookAndFeel(&ItnLookAndFeel::getInstance());
}

EnvelopeGraph::~EnvelopeGraph() {
    setLookAndFeel(nullptr);
}

void EnvelopeGraph::paint(juce::Graphics& g) {
    g.fillAll(GlowStyle::roomDark);

    float attack = *parameters.getRawParameterValue(attackParamName);
    float decay = *parameters.getRawParameterValue(decayParamName);
    float sustain = *parameters.getRawParameterValue(sustainParamName);
    float release = *parameters.getRawParameterValue(releaseParamName);
    float time = env.getNormalizedTime();

    ItnLookAndFeel::drawEnvelope(g, getBounds().toFloat(), attack, decay, sustain, release, time);
}
