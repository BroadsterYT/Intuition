/*
  ==============================================================================

    ItnSlider.h
    Created: 25 Sep 2025 6:48:43pm
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "InlineValueEntry.h"
#include "ItnLookAndFeel.h"


class ItnSlider : public juce::Slider {
public:
    ItnSlider();
    ~ItnSlider();

    void setLabelNames(juce::String newFullName, juce::String newNickname);
    void updateLabel();

    void mouseDown(const juce::MouseEvent& e) override;
    void resized() override;

private:
    ItnLookAndFeel lookAndFeel;
    juce::String fullName;
    juce::String nickname;

    juce::Label label;
};