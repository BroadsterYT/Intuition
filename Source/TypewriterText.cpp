/*
  ==============================================================================

    TypewriterText.cpp
    Created: 8 Jan 2026 7:08:11pm
    Author:  BroDe

  ==============================================================================
*/

#include "TypewriterText.h"

TypewriterText::TypewriterText() {}

void TypewriterText::setText(const juce::String& newText, bool revealAll) {
    text = newText;
    if (revealAll) charsVisible = text.length();
    buildTextLayout();
}

void TypewriterText::setFont(const juce::Font& newFont, const juce::Colour& newColor) {
    font = newFont;
    color = newColor;
    buildTextLayout();
}

float TypewriterText::getFullTextHeight() const {
    return layout.getHeight();
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
    juce::AttributedString partialText;
    partialText.append(text.substring(0, charsVisible), font, color);
    juce::TextLayout partialLayout;
    partialLayout.createLayout(partialText, maxTextWidth);
    auto drawArea = juce::Rectangle<float>(0.0f, 0.0f, layout.getWidth(), layout.getHeight());

    partialLayout.draw(g, drawArea);
}

void TypewriterText::buildTextLayout() {
    attrString.clear();
    attrString.append(text, font, color);
    layout.createLayout(attrString, maxTextWidth);
}
