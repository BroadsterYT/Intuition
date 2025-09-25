/*
  ==============================================================================

    SineWaveSound.h
    Created: 20 Sep 2025 9:20:52pm
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class SineWaveSound : public juce::SynthesiserSound {
public:
    bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
    bool appliesToChannel(int /*midiChannel*/) override { return true; }
};