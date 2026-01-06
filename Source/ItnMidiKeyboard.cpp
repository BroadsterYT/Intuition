/*
  ==============================================================================

    ItnMidiKeyboard.cpp
    Created: 26 Oct 2025 8:04:11pm
    Author:  BroDe

  ==============================================================================
*/

#include "ItnMidiKeyboard.h"


ItnMidiKeyboard::ItnMidiKeyboard(juce::MidiKeyboardState& state, Orientation orientation) : juce::MidiKeyboardComponent(state, orientation) {

}

void ItnMidiKeyboard::drawWhiteNote(int midiNoteNumber, juce::Graphics& g, juce::Rectangle<float> area, bool isDown, bool isOver, juce::Colour lineColour, juce::Colour textColour) {
    /*auto lineColor = juce::ColourGradient::vertical(
        MinimalStyle::shadow.withAlpha(0.5f),
        0.0f,
        MinimalStyle::warmHighlight.withAlpha(0.5f),
        getHeight()
    );*/
    auto lineColor = MinimalStyle::shadow;
    
    float darkRange = 0.5f * std::sin(phase + midiNoteNumber / 12.0f) + 0.5f;
    float darkMult = juce::jmap(darkRange, 0.2f, 1.3f);
    auto baseColor = MinimalStyle::warmHighlight.darker(darkMult);

    auto hoverColor = juce::ColourGradient::vertical(
        MinimalStyle::warmHighlight,
        0.0f,
        MinimalStyle::bulbGlow,
        getHeight()
    );

    /*auto downColor = juce::ColourGradient::vertical(
        MinimalStyle::bulbGlow,
        0.0f,
        MinimalStyle::warmHighlight,
        getHeight()
    );*/
    auto downColor = MinimalStyle::bulbGlow;
    
    if (isDown) {
        g.setColour(downColor);
    }
    else if (isOver) {
        g.setGradientFill(hoverColor);
    }
    else {
        g.setColour(baseColor);
    }
    g.fillRect(area);
}

void ItnMidiKeyboard::drawBlackNote(int midiNoteNumber, juce::Graphics& g, juce::Rectangle<float> area, bool isDown, bool isOver, juce::Colour noteFillColour) {
    auto baseColor = MinimalStyle::roomDark;
    auto hoverColor = juce::ColourGradient::vertical(
        MinimalStyle::warmHighlight,
        0.0f,
        MinimalStyle::bulbGlow,
        getHeight()
    );
    auto downColor = MinimalStyle::bulbGlow;

    if (isDown) {
        g.setColour(downColor);
    }
    else if (isOver) {
        g.setGradientFill(hoverColor);
    }
    else {
        g.setColour(baseColor);
    }

    g.fillRect(area);
}

void ItnMidiKeyboard::timerCallback() {
    phase += 0.12f;
    if (phase > juce::MathConstants<float>::twoPi) {
        phase -= juce::MathConstants<float>::twoPi;
    }
    repaint();
}
