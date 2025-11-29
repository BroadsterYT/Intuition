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
    /// Assigns a new string to the tooltip header (title)
    /// </summary>
    /// <param name="newHeader">The new header to use</param>
    /// <param name="rebuild">Should the text layouts of the tooltip be rebuilt? Defaults to false (still needs to be called to display changes!).</param>
    void setHeader(const juce::String newHeader, bool rebuild = false);
    /// <summary>
    /// Assigns a new string to the tooltip subheader (flavor text)
    /// </summary>
    /// <param name="newSubeader">The new subheader to use</param>
    /// <param name="rebuild">Should the text layouts of the tooltip be rebuilt? Defaults to false (still needs to be called to display changes!).</param>
    void setSubheader(const juce::String newSubheader, bool rebuild = false);
    /// <summary>
    /// Assigns a new string to the tooltip description
    /// </summary>
    /// <param name="newDescription">The new description to use</param>
    /// <param name="rebuild">Should the text layouts of the tooltip be rebuilt? Defaults to false (still needs to be called to display changes!).</param>
    void setDescription(const juce::String newDescription, bool rebuild = false);

    float getFullTextWidth();
    float getFullTextHeight();

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
    float widthPadding = 10.0f;  // The space to save on either side of the text generation

    float topToHeaderPadding = 10.0f;
    float headerToSubheaderPadding = 10.0f;
    float subheaderToDescriptionPadding = 16.0f;
    float descriptionToBottomPadding = 10.0f;

    int headerCharsVisible = 0;
    int subheaderCharsVisible = 0;
    int descriptionCharsVisible = 0;
    int charsPerFrame = 2;

    void buildTextLayouts();
    void timerCallback() override;
};