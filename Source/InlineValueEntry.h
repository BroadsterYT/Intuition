/*
  ==============================================================================

    InlineValueEntry.h
    Created: 25 Sep 2025 7:47:05pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class InlineValueEntry : public juce::Component {
public:
    InlineValueEntry(double currentValue);
    juce::TextEditor editor;

    void linkToSlider(juce::Slider* slider);
    void setExitKeyLambdas();
    void resized() override;

private:
    juce::Slider* slider = nullptr;
};