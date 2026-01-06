/*
  ==============================================================================

    EnvelopeDisplay.h
    Created: 21 Sep 2025 5:57:23pm
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnSlider.h"
#include "EnvelopeGraph.h"
#include "EnvelopeManager.h"
#include "SliderComponent.h"


class EnvelopeDisplay : public juce::Component, private juce::Timer {
public:
    EnvelopeDisplay(
        juce::AudioProcessorValueTreeState& vts,
        const GlobalEnvelope& env,
        const juce::String attackParamName,
        const juce::String decayParamName,
        const juce::String sustainParamName,
        const juce::String releaseParamName
    );
    ~EnvelopeDisplay() override;

    void visibilityChanged() override;
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    const GlobalEnvelope& env;

    juce::String attackParamName;
    juce::String decayParamName;
    juce::String sustainParamName;
    juce::String releaseParamName;

    SliderComponent attackSlider, decaySlider, sustainSlider, releaseSlider;
    /*ItnSlider attackSlider, decaySlider, sustainSlider, releaseSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment, decayAttachment, sustainAttachment, releaseAttachment;*/

    EnvelopeGraph graph;

    void drawEnvelopeGraph(juce::Graphics& g);

    void timerCallback() override;
};