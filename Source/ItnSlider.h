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


class ItnSlider : public juce::Slider {
public:
    void mouseDown(const juce::MouseEvent& e) override;
};