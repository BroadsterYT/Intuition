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


class EnvelopeDisplay : public juce::Component,
    private juce::AudioProcessorValueTreeState::Listener{
public:
    EnvelopeDisplay(
        juce::AudioProcessorValueTreeState& vts,
        const juce::String attackParamName,
        const juce::String decayParamName,
        const juce::String sustainParamName,
        const juce::String releaseParamName
    );
    ~EnvelopeDisplay() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    juce::String attackParamName;
    juce::String decayParamName;
    juce::String sustainParamName;
    juce::String releaseParamName;

    ItnSlider attackSlider, decaySlider, sustainSlider, releaseSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment, decayAttachment, sustainAttachment, releaseAttachment;

    void drawEnvelopeGraph(juce::Graphics& g);
    void parameterChanged(const juce::String&, float) override;
};