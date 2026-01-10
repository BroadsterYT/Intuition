/*
  ==============================================================================

    TypewriterText.h
    Created: 8 Jan 2026 7:08:11pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnLookAndFeel.h"


class TypewriterText : public juce::Component {
public:
    TypewriterText();

    /// <summary>
    /// Sets the text to display in this component.
    /// </summary>
    /// <param name="newText">The text to display</param>
    /// <param name="revealAll">Should all characters be visible when assigned? Defaults to false.</param>
    void setText(const juce::String& newText, bool revealAll = false);
    /// <summary>
    /// Sets the font and color of the text that is displayed in this component.
    /// </summary>
    /// <param name="newFont">The font to use for the text</param>
    /// <param name="newColor">The color the text should be</param>
    void setFont(const juce::Font& newFont, const juce::Colour& newColor = MinimalStyle::textPrimary);
    float getFullTextHeight() const;
    /// <summary>
    /// Performs an iteration of the typewriter effect.
    /// </summary>
    /// <param name="repaintNeeded">If true, will repaint this component if new characters are made visible.</param>
    /// <returns>True if new characters were revealed, false if all characters have been revealed.</returns>
    bool iterateTypewriterEffect(bool repaintNeeded = false);

    void paint(juce::Graphics& g) override;

private:
    juce::String text;
    juce::AttributedString attrString;
    juce::TextLayout layout;

    juce::Font font = ItnLookAndFeel::getInstance().getTooltipDescriptionFont();
    juce::Colour color = MinimalStyle::textPrimary;

    int charsVisible = 0;
    int charsPerFrame = 2;  // How many characters to reveal every iteration

    void buildTextLayout();
};
