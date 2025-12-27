/*
  ==============================================================================

    SliderComponent.h
    Created: 17 Dec 2025 10:13:02pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ModMatrix.h"
#include "ItnSlider.h"


/// <summary>
/// A component that holds an ItnSlider, a label, and an editable value box for it.
/// </summary>
class SliderComponent : public juce::Component {
public:
    SliderComponent(juce::AudioProcessorValueTreeState& vts, const juce::String paramName, const juce::String displayName, const juce::String units = "");

    void setModMatrix(ModMatrix* matrix);
    /// <summary>
    /// Sets the limits that this component's slider can take.
    /// </summary>
    /// <param name="newMinimum">The lowest value allowed</param>
    /// <param name="newMaximum">The highest value allowed</param>
    /// <param name="newInterval">The steps in which the value is allowed to increase</param>
    void setRange(double newMinimum, double newMaximum, double newInterval = 0.0);
    void setSkewFactorFromMidPoint(double sliderValueToShowAtMidPoint);
    
    void setCustomTooltipText(const juce::String parameterKey);

    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;

    ItnSlider slider;
    juce::Label nameLabel;
    juce::Label valueBox;
    juce::String units;

    // Slider parameter linking
    juce::String paramName;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;

    /// <summary>
    /// Returns the formatted string to display for the slider's valueBox
    /// </summary>
    /// <returns>The formatted juce::String</returns>
    juce::String formatValueWithUnit(double v);
};
