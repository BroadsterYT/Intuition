/*
  ==============================================================================

    DelayDisplay.cpp
    Created: 14 Nov 2025 6:57:38pm
    Author:  BroDe

  ==============================================================================
*/

#include "DelayDisplay.h"

DelayDisplay::DelayDisplay(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix) : parameters(vts), modMatrix(modMatrix) {
    setLookAndFeel(&lookAndFeel);
}
