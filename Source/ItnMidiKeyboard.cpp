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
        GlowStyle::shadow.withAlpha(0.5f),
        0.0f,
        GlowStyle::warmHighlight.withAlpha(0.5f),
        getHeight()
    );*/
    auto lineColor = GlowStyle::shadow;
    
    float darkRange = 0.5f * std::sin(phase + midiNoteNumber / 12.0f) + 0.5f;
    float darkMult = juce::jmap(darkRange, 0.2f, 1.3f);
    auto baseColor = GlowStyle::warmHighlight.darker(darkMult);

    auto hoverColor = juce::ColourGradient::vertical(
        GlowStyle::warmHighlight,
        0.0f,
        GlowStyle::bulbGlow,
        getHeight()
    );

    /*auto downColor = juce::ColourGradient::vertical(
        GlowStyle::bulbGlow,
        0.0f,
        GlowStyle::warmHighlight,
        getHeight()
    );*/
    auto downColor = GlowStyle::bulbGlow;
    
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
