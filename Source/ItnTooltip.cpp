/*
  ==============================================================================

    ItnTooltip.cpp
    Created: 26 Nov 2025 4:56:45pm
    Author:  BroDe

  ==============================================================================
*/

#include "ItnTooltip.h"

ItnTooltip::ItnTooltip() {
    setLookAndFeel(&ItnLookAndFeel::getInstance());
    setAlwaysOnTop(true);
    setInterceptsMouseClicks(false, false);
    setMouseClickGrabsKeyboardFocus(false);
    startTimerHz(60);

    headerFont = juce::Font(juce::FontOptions("Arial", 24.0f, juce::Font::bold));
    subheaderFont = juce::Font(juce::FontOptions("Arial", 16.0f, juce::Font::italic));
    descriptionFont = juce::Font(juce::FontOptions("Arial", 16.0f, juce::Font::plain));

    buildTextLayouts();
}

ItnTooltip::~ItnTooltip() {
    setLookAndFeel(nullptr);
    stopTimer();
}

void ItnTooltip::show(const int x, const int y) {
    headerCharsVisible = 0;
    subheaderCharsVisible = 0;
    descriptionCharsVisible = 0;

    setBounds(x, y, 480, 480);
    setVisible(true);
}

void ItnTooltip::hide() {
    setVisible(false);
}

void ItnTooltip::setText(const juce::String newHeader, const juce::String newSubheader, const juce::String newDescription) {
    header = newHeader;
    subheader = newSubheader;
    description = newDescription;

    buildTextLayouts();
}

void ItnTooltip::paint(juce::Graphics& g) {
    auto area = getLocalBounds().toFloat();

    float headerY = topToHeaderPadding;
    float subheaderY = headerY + headerLayout.getHeight() + headerToSubheaderPadding;
    float descriptionY = subheaderY + subheaderLayout.getHeight() + subheaderToDescriptionPadding;
    float totalHeight = descriptionY + descriptionLayout.getHeight() + descriptionToBottomPadding;

    float widthPadding = 10.0f;
    auto filledArea = area.removeFromTop(totalHeight).removeFromLeft(maxTextWidth + 2.0f * widthPadding);

    g.setColour(GlowStyle::shadow);
    g.fillRoundedRectangle(filledArea, 15.0f);
    g.setColour(GlowStyle::warmHighlight);
    g.drawRoundedRectangle(filledArea, 15.0f, 2.0f);

    auto drawPartialLayout = [&](const juce::String& text, int charCount, float posY, juce::Font& font, juce::Colour color) {
        juce::AttributedString partialText;
        partialText.append(text.substring(0, charCount), font, color);
        juce::TextLayout layout;
        layout.createLayout(partialText, maxTextWidth);
        layout.draw(g, juce::Rectangle<float>(widthPadding, posY, layout.getWidth(), layout.getHeight()));
    };

    // Drawing partial text layouts for typewriter effect
    if (headerCharsVisible > 0) {
        drawPartialLayout(header, headerCharsVisible, headerY, headerFont, GlowStyle::bulbGlow);
    }
    if (subheaderCharsVisible > 0) {
        drawPartialLayout(subheader, subheaderCharsVisible, subheaderY, subheaderFont, GlowStyle::bulbGlow);
    }
    if (descriptionCharsVisible > 0) {
        drawPartialLayout(description, descriptionCharsVisible, descriptionY, descriptionFont, GlowStyle::bulbGlow);
    }
}

void ItnTooltip::buildTextLayouts() {
    headerText.clear();
    subheaderText.clear();
    descriptionText.clear();

    headerText.append(header, headerFont, GlowStyle::bulbGlow);
    subheaderText.append(subheader, subheaderFont, GlowStyle::bulbGlow);
    descriptionText.append(description, descriptionFont, GlowStyle::bulbGlow);

    headerLayout.createLayout(headerText, maxTextWidth);
    subheaderLayout.createLayout(subheaderText, maxTextWidth);
    descriptionLayout.createLayout(descriptionText, maxTextWidth);
}

float ItnTooltip::getTotalTextHeight() {
    return 0.0f;
}

void ItnTooltip::timerCallback() {
    bool repaintNeeded = false;

    int headerLen = header.length();
    int subheaderLen = subheader.length();
    int descLen = description.length();

    if (headerCharsVisible < headerLen) {
        headerCharsVisible += charsPerFrame;
        if (headerCharsVisible > headerLen) headerCharsVisible = headerLen;
        repaintNeeded = true;
    }
    else if (subheaderCharsVisible < subheaderLen) {
        subheaderCharsVisible += charsPerFrame;
        if (subheaderCharsVisible > subheaderLen) subheaderCharsVisible = subheaderLen;
        repaintNeeded = true;
    }
    else if (descriptionCharsVisible < descLen) {
        descriptionCharsVisible += charsPerFrame;
        if (descriptionCharsVisible > descLen) descriptionCharsVisible = descLen;
        repaintNeeded = true;
    }

    if (repaintNeeded) repaint();
}
