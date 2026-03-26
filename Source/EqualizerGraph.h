/*
  ==============================================================================

    EqualizerGraph.h
    Created: 21 Feb 2026 1:23:40pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnLookAndFeel.h"
#include "ModMatrix.h"
#include "EqualizerModule.h"


class EqualizerGraph : public juce::Component, private juce::Timer {
public:
    EqualizerGraph(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix, EqualizerModule& equalizer);
    ~EqualizerGraph();

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;

    /// <summary>
    /// Returns the nearest EQ Band to the mouse cursor
    /// </summary>
    /// <param name="e">The mouse event to listen on</param>
    /// <returns>The EQ band nearest the mouse cursor, if within range. Otherwise, returns nullptr</returns>
    const EQBand* getNearestEQBand(const juce::MouseEvent& e);

    void paint(juce::Graphics& g) override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;

    EqualizerModule& equalizer;

    const EQBand* currentDraggedBand = nullptr;

    void timerCallback() override;
};
