/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 21 Sep 2025 5:57:40pm
    Author:  Brody

  ==============================================================================
*/

#include "ADSRComponent.h"
#include <algorithm>


ADSRComponent::ADSRComponent(juce::AudioProcessorValueTreeState& vts) : parameters(vts) {
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(sustainSlider);
    addAndMakeVisible(releaseSlider);

    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "DECAY", decaySlider);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "RELEASE", releaseSlider);

    attackSlider.setSliderStyle(juce::Slider::Rotary);
    decaySlider.setSliderStyle(juce::Slider::Rotary);
    sustainSlider.setSliderStyle(juce::Slider::Rotary);
    releaseSlider.setSliderStyle(juce::Slider::Rotary);

    attackSlider.setTooltip("Attack");
    attackSlider.setPopupDisplayEnabled(true, false, this);
    decaySlider.setTooltip("Decay");
    decaySlider.setPopupDisplayEnabled(true, false, this);
    sustainSlider.setTooltip("Sustain");
    sustainSlider.setPopupDisplayEnabled(true, false, this);
    releaseSlider.setTooltip("Release");
    releaseSlider.setPopupDisplayEnabled(true, false, this);

    parameters.addParameterListener("ATTACK", this);
    parameters.addParameterListener("DECAY", this);
    parameters.addParameterListener("SUSTAIN", this);
    parameters.addParameterListener("RELEASE", this);

    attackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    decaySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    sustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    releaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);

    attackLabel.setText("A", juce::dontSendNotification);
    decayLabel.setText("D", juce::dontSendNotification);
    sustainLabel.setText("S", juce::dontSendNotification);
    releaseLabel.setText("R", juce::dontSendNotification);

    attackLabel.setJustificationType(juce::Justification::centredBottom);
    decayLabel.setJustificationType(juce::Justification::centredBottom);
    sustainLabel.setJustificationType(juce::Justification::centredBottom);
    releaseLabel.setJustificationType(juce::Justification::centredBottom);

    addAndMakeVisible(attackLabel);
    addAndMakeVisible(decayLabel);
    addAndMakeVisible(sustainLabel);
    addAndMakeVisible(releaseLabel);
}

ADSRComponent::~ADSRComponent() {
    parameters.removeParameterListener("ATTACK", this);
    parameters.removeParameterListener("DECAY", this);
    parameters.removeParameterListener("SUSTAIN", this);
    parameters.removeParameterListener("RELEASE", this);
}

void ADSRComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::darkgrey);
    g.fillRect(getLocalBounds());
    
    g.setColour(juce::Colours::white);
    drawEnvelopeGraph(g);
}

void ADSRComponent::resized() {
    auto area = getLocalBounds().reduced(10);
    auto knobArea = area.removeFromBottom(80);
    auto labelArea = knobArea.removeFromBottom(15);

    int labelWidth = labelArea.getWidth() / 4;
    attackLabel.setBounds(labelArea.removeFromLeft(labelWidth));
    decayLabel.setBounds(labelArea.removeFromLeft(labelWidth));
    sustainLabel.setBounds(labelArea.removeFromLeft(labelWidth));
    releaseLabel.setBounds(labelArea.removeFromLeft(labelWidth));

    int sliderWidth = knobArea.getWidth() / 4;
    attackSlider.setBounds(knobArea.removeFromLeft(sliderWidth));
    decaySlider.setBounds(knobArea.removeFromLeft(sliderWidth));
    sustainSlider.setBounds(knobArea.removeFromLeft(sliderWidth));
    releaseSlider.setBounds(knobArea.removeFromLeft(sliderWidth));
}

void ADSRComponent::drawEnvelopeGraph(juce::Graphics& g) {
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

void ADSRComponent::parameterChanged(const juce::String&, float) {
    repaint();
}