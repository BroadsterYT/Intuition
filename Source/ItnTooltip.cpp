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
}

ItnTooltip::~ItnTooltip() {
    setLookAndFeel(nullptr);
    stopTimer();
}

void ItnTooltip::show(const int x, const int y) {
    DBG("Tooltip shown!");
    DBG("X: " << x << " Y: " << y);
    
    setBounds(x, y, 480, 240);
    setVisible(true);
}

void ItnTooltip::hide() {
    setVisible(false);
}

void ItnTooltip::setHeader(const juce::String newHeader) {
    header = newHeader;
}

void ItnTooltip::setSubheader(const juce::String newSubheader) {
    subheader = newSubheader;
}

void ItnTooltip::setDescription(const juce::String newDescription) {
    description = newDescription;
}

void ItnTooltip::paint(juce::Graphics& g) {
    auto area = getLocalBounds().toFloat();

    // Header
    juce::Font headerFont("Arial", 24.0f, juce::Font::bold);
    juce::AttributedString headerText;
    headerText.append(header, headerFont, GlowStyle::bulbGlow);
    juce::TextLayout headerLayout;
    headerLayout.createLayout(headerText, maxTextWidth);

    // Subheader
    juce::Font subheaderFont("Arial", 18.0f, juce::Font::italic);
    juce::AttributedString subheaderText;
    subheaderText.append(subheader, subheaderFont, GlowStyle::bulbGlow);
    juce::TextLayout subheaderLayout;
    subheaderLayout.createLayout(subheaderText, maxTextWidth);

    // Description
    juce::Font descriptionFont("Arial", 12.0f, juce::Font::bold);
    juce::AttributedString descriptionText;
    descriptionText.append(description, descriptionFont, GlowStyle::bulbGlow);
    juce::TextLayout descriptionLayout;
    descriptionLayout.createLayout(descriptionText, maxTextWidth);

    float topToHeaderPadding = 10.0f;
    float headerToSubheaderPadding = 10.0f;
    float subheaderToDescriptionPadding = 20.0f;
    float descriptionToBottomPadding = 10.0f;

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

    // Drawing all to tooltip box
    headerLayout.draw(g, juce::Rectangle<float>(widthPadding, headerY, headerLayout.getWidth(), headerLayout.getHeight()));
    subheaderLayout.draw(g, juce::Rectangle<float>(widthPadding, subheaderY, subheaderLayout.getWidth(), subheaderLayout.getHeight()));
    descriptionLayout.draw(g, juce::Rectangle<float>(widthPadding, descriptionY, descriptionLayout.getWidth(), descriptionLayout.getHeight()));
}

void ItnTooltip::timerCallback() {
    //repaint();
}
