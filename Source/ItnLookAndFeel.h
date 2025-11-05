/*
  ==============================================================================

    ItnLookAndFeel.h
    Created: 29 Sep 2025 1:07:41am
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnLookAndFeelHelpers.h"


class ItnLookAndFeel : public juce::LookAndFeel_V4 {
public:
    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override;

    void drawRotarySlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider& slider
    ) override;

    void drawWaveform(juce::Graphics& g,
        juce::Rectangle<float> bounds,
        const juce::Array<float> waveform
    );
    void drawEnvelope(juce::Graphics& g,
        juce::Rectangle<float> bounds,
        float attack, float decay, float sustain, float release,
        float envTime
    );
    void drawLFO(juce::Graphics& g,
        juce::Rectangle<float> bounds,
        LFOShape& shape,
        float phase
    );
    void drawLFOPoint(juce::Graphics& g,
        juce::Rectangle<float> bounds,
        const LFOPoint& point
    );
    void drawFilter(juce::Graphics& g,
        juce::Rectangle<float> bounds,
        float cutoff, float resonance, int filterType
    );
};