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
    auto lineColor = MinimalStyle::bgDark;
    
    float darkRange = 0.5f * std::sin(phase + midiNoteNumber / 12.0f) + 0.5f;
    float darkMult = juce::jmap(darkRange, 0.2f, 1.3f);
    auto baseColor = MinimalStyle::accentPeach.darker(darkMult);

    auto hoverColor = juce::ColourGradient::vertical(
        MinimalStyle::accentGlow,
        0.0f,
        MinimalStyle::accentOrange,
        getHeight()
    );
    auto downColor = juce::Colours::white;
    
    if (isDown) {
        g.setColour(downColor);
    }
    else if (isOver) {
        g.setGradientFill(hoverColor);
    }
    else {
        g.setColour(baseColor);
    }
    g.fillRect(area.expanded(1.0f));
}

void ItnMidiKeyboard::drawBlackNote(int midiNoteNumber, juce::Graphics& g, juce::Rectangle<float> area, bool isDown, bool isOver, juce::Colour noteFillColour) {
    auto baseColor = MinimalStyle::bgDark;
    auto hoverColor = juce::ColourGradient::vertical(
        MinimalStyle::accentGlow.withAlpha(1.0f),
        0.0f,
        MinimalStyle::accentOrange,
        getHeight()
    );
    auto downColor = juce::Colours::white;

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
