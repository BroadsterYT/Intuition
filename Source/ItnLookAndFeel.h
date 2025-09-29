/*
  ==============================================================================

    ItnLookAndFeel.h
    Created: 29 Sep 2025 1:07:41am
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class ItnLookAndFeel : public juce::LookAndFeel_V4 {
public:
    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override;
};