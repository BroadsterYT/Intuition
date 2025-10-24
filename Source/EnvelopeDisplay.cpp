/*
  ==============================================================================

    EnvelopeDisplay.cpp
    Created: 21 Sep 2025 5:57:40pm
    Author:  Brody

  ==============================================================================
*/

#include "EnvelopeDisplay.h"
#include <algorithm>


EnvelopeDisplay::EnvelopeDisplay(juce::AudioProcessorValueTreeState& vts) : parameters(vts) {
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(releaseSlider);

    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "DECAY", decaySlider);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "RELEASE", releaseSlider);

    attackSlider.setLabelNames("Attack", "A");
    decaySlider.setLabelNames("Decay", "D");
    sustainSlider.setLabelNames("Sustain", "S");
    releaseSlider.setLabelNames("Release", "R");

    parameters.addParameterListener("ATTACK", this);
    parameters.addParameterListener("DECAY", this);
    parameters.addParameterListener("SUSTAIN", this);
    parameters.addParameterListener("RELEASE", this);
}

EnvelopeDisplay::~EnvelopeDisplay() {
    parameters.removeParameterListener("ATTACK", this);
    parameters.removeParameterListener("DECAY", this);
    parameters.removeParameterListener("SUSTAIN", this);
    parameters.removeParameterListener("RELEASE", this);
}

void EnvelopeDisplay::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::darkgrey);
    g.fillRect(getLocalBounds());
    
    g.setColour(juce::Colours::white);
    drawEnvelopeGraph(g);
}

void EnvelopeDisplay::resized() {
    auto area = getLocalBounds().reduced(10);
    auto knobArea = area.removeFromBottom(80);

    int sliderWidth = knobArea.getWidth() / 4;
    attackSlider.setBounds(knobArea.removeFromLeft(sliderWidth));
    decaySlider.setBounds(knobArea.removeFromLeft(sliderWidth));
    sustainSlider.setBounds(knobArea.removeFromLeft(sliderWidth));
    releaseSlider.setBounds(knobArea.removeFromLeft(sliderWidth));
}

void EnvelopeDisplay::drawEnvelopeGraph(juce::Graphics& g) {
    float attack = *parameters.getRawParameterValue("ATTACK");
    float decay = *parameters.getRawParameterValue("DECAY");
    float sustain = *parameters.getRawParameterValue("SUSTAIN");
    float release = *parameters.getRawParameterValue("RELEASE");

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

void EnvelopeDisplay::parameterChanged(const juce::String&, float) {
    repaint();
}