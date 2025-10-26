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
}

void EnvelopeGraph::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    drawEnvelopePath(g);
}

void EnvelopeGraph::drawSecondMarkTicks(juce::Graphics& g) {
    g.setColour(juce::Colours::dimgrey);
    g.setOpacity(0.5f);

    float totalTime = 3.0f; // TODO: Replace with time multiplier parameter

    int numTicks = std::floor(totalTime);
    for (int i = 1; i <= numTicks; ++i) {
        float tickX = getWidth() * (i / totalTime);
        g.drawLine(tickX, 0.0f, tickX, getHeight(), 1.0f);
    }
}

void EnvelopeGraph::drawEnvelopePath(juce::Graphics& g) {
    float attack = *parameters.getRawParameterValue(attackParamName);
    float decay = *parameters.getRawParameterValue(decayParamName);
    float sustain = *parameters.getRawParameterValue(sustainParamName);
    float release = *parameters.getRawParameterValue(releaseParamName);

    int width = getWidth();
    int height = getHeight();

    float segWidth = (float)getWidth() / 3;  // TODO: Replace with time multiplier

    float attackX = attack * segWidth;
    float decayX = decay * segWidth;
    float sustainY = height * (1.0f - sustain);
    float releaseX = release * segWidth;

    float decayEndX = attackX + decayX;
    float releaseEndX = decayEndX + releaseX;

    juce::Path envPolygon;
    envPolygon.startNewSubPath(0.0f, height);
    envPolygon.lineTo(attackX, 0.0f);
    envPolygon.lineTo(decayEndX, sustainY);
    envPolygon.lineTo(releaseEndX, height);
    envPolygon.closeSubPath();

    juce::ColourGradient envGrad = juce::ColourGradient::vertical(
        juce::Colours::darkgoldenrod.withAlpha(0.8f),
        0.0f, 
        juce::Colours::darkgoldenrod.withAlpha(0.0f),
        height
    );
    g.setGradientFill(envGrad);
    g.fillPath(envPolygon);

    g.setColour(juce::Colours::lightgoldenrodyellow);
    juce::Path envLine;
    envLine.startNewSubPath(0.0f, height);
    envLine.lineTo(attackX, 0.0f);
    envLine.lineTo(decayEndX, sustainY);
    envLine.lineTo(releaseEndX, height);

    g.setColour(juce::Colours::lightgoldenrodyellow);
    g.strokePath(envLine, juce::PathStrokeType(2.0f));

    // ----- Drawing Envelope position ----- //
    g.setColour(juce::Colours::limegreen);

    float time = env.getNormalizedTime();
    if (time <= 0.0f) return;
    float timeSeg = time * segWidth;

    float finalX = timeSeg * (attack + decay + release);
    g.drawLine(finalX, height, finalX, 0.0f, 2.0f);
}
