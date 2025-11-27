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

    /// <summary>
    /// Sets all text fields of the tooltip to new values
    /// </summary>
    /// <param name="newHeader">The new header text to display</param>
    /// <param name="newSubheader">The new subheader to display</param>
    /// <param name="newDescription">The new description to display</param>
    void setText(const juce::String newHeader, const juce::String newSubheader, const juce::String newDescription);

    void paint(juce::Graphics& g) override;

private:
    juce::String header = "Lorem Ipsum";
    juce::String subheader = "dolor sit amet";
    juce::String description = "Consectetur adipiscing elit. Nullam eu eleifend lorem. Nam rutrum sodales accumsan. Nunc eu diam ut.";

    juce::Font headerFont;
    juce::Font subheaderFont;
    juce::Font descriptionFont;

    juce::AttributedString headerText;
    juce::AttributedString subheaderText;
    juce::AttributedString descriptionText;

    juce::TextLayout headerLayout;
    juce::TextLayout subheaderLayout;
    juce::TextLayout descriptionLayout;

    float maxTextWidth = 300.0f;
    float topToHeaderPadding = 10.0f;
    float headerToSubheaderPadding = 10.0f;
    float subheaderToDescriptionPadding = 16.0f;
    float descriptionToBottomPadding = 10.0f;

    int headerCharsVisible = 0;
    int subheaderCharsVisible = 0;
    int descriptionCharsVisible = 0;
    int charsPerFrame = 2;

    void buildTextLayouts();
    /// <summary>
    /// Returns the height of all text fields combined with all vertical padding
    /// </summary>
    /// <returns>The height of all text elements in the tooltip</returns>
    float getTotalTextHeight();
    void timerCallback() override;
};