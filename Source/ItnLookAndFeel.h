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
    ItnLookAndFeel();
    static ItnLookAndFeel& getInstance();

    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override;

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
        juce::Slider& slider) override;

    juce::Font getTooltipHeaderFont(float height = 36.0f);
    juce::Font getTooltipSubheaderFont(float height = 20.0f);
    juce::Font getTooltipDescriptionFont(float height = 20.0f);

    static void drawWaveform(juce::Graphics& g,juce::Rectangle<float> bounds, const juce::Array<float> waveform);
    static void drawEnvelope(juce::Graphics& g, juce::Rectangle<float> bounds, float attack, float decay, float sustain, float release, float envTime);
    static void drawLFO(juce::Graphics& g, juce::Rectangle<float> bounds, LFOShape& shape, float phase);
    static void drawLFOPoint(juce::Graphics& g, juce::Rectangle<float> bounds, const LFOPoint& point);
    static void drawFilter(juce::Graphics& g, juce::Rectangle<float> bounds, float cutoff, float resonance, int filterType);

private:
    juce::Typeface::Ptr exo2TypeFaceRegular;
    juce::Typeface::Ptr exo2TypeFaceBold;
    juce::Typeface::Ptr exo2TypeFaceItalic;
};