/*
  ==============================================================================

    ADSRComponent.h
    Created: 21 Sep 2025 5:57:23pm
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class ADSRComponent : public juce::Component,
    private juce::AudioProcessorValueTreeState::Listener{
public:
    ADSRComponent(juce::AudioProcessorValueTreeState& vts);
    ~ADSRComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;

    juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment, decayAttachment, sustainAttachment, releaseAttachment;
    juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel;

    void drawEnvelopeGraph(juce::Graphics& g);
    void parameterChanged(const juce::String&, float) override;
};