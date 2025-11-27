/*
  ==============================================================================

    EnvelopeDisplay.cpp
    Created: 21 Sep 2025 5:57:40pm
    Author:  Brody

  ==============================================================================
*/

#include "EnvelopeDisplay.h"
#include <algorithm>


EnvelopeDisplay::EnvelopeDisplay(
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
    releaseParamName(releaseParamName),
    graph(vts, env, attackParamName, decayParamName, sustainParamName, releaseParamName) {
    
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(releaseSlider);

    addAndMakeVisible(graph);

    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, attackParamName, attackSlider);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, decayParamName, decaySlider);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, sustainParamName, sustainSlider);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, releaseParamName, releaseSlider);

    attackSlider.setLabelNames("Attack", "A");
    decaySlider.setLabelNames("Decay", "D");
    sustainSlider.setLabelNames("Sustain", "S");
    releaseSlider.setLabelNames("Release", "R");

    attackSlider.setTooltipFields(
        "Attack",
        "Because why be punctual?",
        "Adjusts the length of the entrance ramp.\n\nFor example, let's say the envelope is modulating the volume of an oscillator (which ENV_OSC is hard-linked to do). An attack of 0 will cause notes to lead directly into their decay, but an attack of 1 will slowly ramp up to full volume before decaying."
    );

    decaySlider.setTooltipFields(
        "Decay",
        "A graceful retreat... or a quick flop.",
        "The decay is the downward ramp between the attack and the sustain - the transition from the peak to the trough. A larger decay will fade slower from the max to the sustain."
    );

    sustainSlider.setTooltipFields(
        "Sustain",
        "The part of the note that refuses to leave.",
        "The sustain part of the envelope is held until all notes have been released. If no attack or decay is set, then notes begin immediately at the sustain."
    );

    releaseSlider.setTooltipFields(
        "Release",
        "Notes need closure, too! (# > <)",
        "The release is the fadeout of the envelope. The longer the release, the longer the envelope will persist after all notes have been released."
    );
}

EnvelopeDisplay::~EnvelopeDisplay() {
    stopTimer();
}

void EnvelopeDisplay::visibilityChanged() {
    if (isShowing()) {
        startTimerHz(60);
    }
    else {
        stopTimer();
    }
}

void EnvelopeDisplay::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::darkgrey);
    g.fillRect(getLocalBounds());
    
    g.setColour(juce::Colours::white);
    //drawEnvelopeGraph(g);
}

void EnvelopeDisplay::resized() {
    auto area = getLocalBounds().reduced(10);
    auto knobArea = area.removeFromBottom(80);

    int sliderWidth = knobArea.getWidth() / 4;
    attackSlider.setBounds(knobArea.removeFromLeft(sliderWidth));
    decaySlider.setBounds(knobArea.removeFromLeft(sliderWidth));
    sustainSlider.setBounds(knobArea.removeFromLeft(sliderWidth));
    releaseSlider.setBounds(knobArea.removeFromLeft(sliderWidth));

    area.removeFromBottom(10);
    graph.setBounds(area);
}

void EnvelopeDisplay::drawEnvelopeGraph(juce::Graphics& g) {
    float attack = *parameters.getRawParameterValue(attackParamName);
    float decay = *parameters.getRawParameterValue(decayParamName);
    float sustain = *parameters.getRawParameterValue(sustainParamName);
    float release = *parameters.getRawParameterValue(releaseParamName);

    auto bounds = getLocalBounds().toFloat().reduced(5);

    float totalTime = attack + decay + release + 0.001f;  // No divide by 0
    float xAttack = bounds.getX() + bounds.getWidth() * (attack / totalTime);
    float xDecay = xAttack + bounds.getWidth() * (decay / totalTime);
    float xSustain = std::clamp<float>(xDecay + bounds.getWidth() * 0.05f, 0.0, bounds.getRight());
    float xRelease = bounds.getRight();

    float yStart = bounds.getBottom();
    float yPeak = bounds.getY();
    float ySustain = bounds.getBottom() - bounds.getHeight() * sustain;

    juce::Path env;
    env.startNewSubPath(bounds.getX(), yStart);
    env.lineTo(xAttack, yPeak);
    env.lineTo(xDecay, ySustain);
    env.lineTo(xSustain, ySustain);
    env.lineTo(xRelease, yStart);

    g.strokePath(env, juce::PathStrokeType(2.0f));
}

void EnvelopeDisplay::timerCallback() {
    repaint();
}