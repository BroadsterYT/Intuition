/*
  ==============================================================================

    ItnMidiKeyboard.h
    Created: 26 Oct 2025 8:04:11pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnLookAndFeel.h"


class ItnMidiKeyboard : public juce::MidiKeyboardComponent {
public:
    ItnMidiKeyboard(juce::MidiKeyboardState& state, Orientation orientation);

    void drawWhiteNote(
        int midiNoteNumber,
        juce::Graphics& g,
        juce::Rectangle<float> area,
        bool isDown,
        bool isOver,
        juce::Colour lineColour,
        juce::Colour textColour) override;

private:
    float phase = 0.0f;
    void timerCallback() override;
};