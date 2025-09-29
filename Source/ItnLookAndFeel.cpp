/*
  ==============================================================================

    ItnLookAndFeel.cpp
    Created: 29 Sep 2025 1:08:01am
    Author:  BroDe

  ==============================================================================
*/

#include "ItnLookAndFeel.h"


juce::Slider::SliderLayout ItnLookAndFeel::getSliderLayout(juce::Slider& slider) {
    juce::Slider::SliderLayout layout;

    auto bounds = slider.getLocalBounds();
    layout.sliderBounds = bounds;

    if (slider.getTextBoxPosition() == juce::Slider::TextBoxAbove) {
        auto textArea = bounds.removeFromTop(slider.getTextBoxHeight());
        auto labelArea = bounds.removeFromBottom(10);
        float textBoxPadding = bounds.getWidth() - slider.getTextBoxWidth();
        textArea.removeFromLeft(textBoxPadding / 2);
        textArea.removeFromRight(textBoxPadding / 2);

        layout.sliderBounds = bounds;
        layout.textBoxBounds = textArea;
    }
    else if (slider.getTextBoxPosition() == juce::Slider::NoTextBox) {
        auto labelArea = bounds.removeFromBottom(10);
        slider.setPopupDisplayEnabled(true, false, slider.getParentComponent());

        layout.sliderBounds = bounds;
    }

    return layout;
}