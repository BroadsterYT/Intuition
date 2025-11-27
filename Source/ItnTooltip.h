/*
  ==============================================================================

    ItnTooltip.h
    Created: 26 Nov 2025 4:56:45pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnLookAndFeel.h"


class ItnTooltip : public juce::Component, private juce::Timer {
public:
    ItnTooltip();
    ~ItnTooltip();

    void show(const int x, const int y);
    void hide();

    void setHeader(const juce::String newHeader);
    void setSubheader(const juce::String newSubheader);
    void setDescription(const juce::String newDescription);

    void paint(juce::Graphics& g) override;

private:
    juce::String header = "Lorem Ipsum";
    juce::String subheader = "dolor sit amet";
    juce::String description = "Consectetur adipiscing elit. Nullam eu eleifend lorem. Nam rutrum sodales accumsan. Nunc eu diam ut.";

    float maxTextWidth = 200.0f;

    void timerCallback() override;
};