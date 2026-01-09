/*
  ==============================================================================

    TypewriterText.cpp
    Created: 8 Jan 2026 7:08:11pm
    Author:  BroDe

  ==============================================================================
*/

#include "TypewriterText.h"

TypewriterText::TypewriterText() {}

void TypewriterText::setText(const juce::String& newText) {
    text = newText;
    buildTextLayout();
}

void TypewriterText::setFont(const juce::Font& newFont, const juce::Colour& newColor) {
    font = newFont;
    color = newColor;
    buildTextLayout();
}

bool TypewriterText::iterateTypewriterEffect(bool repaintNeeded) {
    int textLen = text.length();

    if (charsVisible < textLen) {
        charsVisible += charsPerFrame;
        if (repaintNeeded) repaint();
        return true;
    }
    else {
        charsVisible = textLen;
        return false;
    }
}

void TypewriterText::paint(juce::Graphics& g) {
    float textWidth = getLocalBounds().toFloat().getWidth();
    ItnLookAndFeel* lookAndFeel = &ItnLookAndFeel::getInstance();

    juce::AttributedString partialText;
    partialText.append(text.substring(0, charsVisible), font, color);
    juce::TextLayout partialLayout;
    partialLayout.createLayout(partialText, textWidth);
    partialLayout.draw(g, getLocalBounds().toFloat());
}

void TypewriterText::buildTextLayout() {
    attrString.clear();
    attrString.append(text, font, color);
    layout.createLayout(attrString, getLocalBounds().getWidth());
}
