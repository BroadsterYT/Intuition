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
    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override;

    /// <summary>
    /// Returns the nearest EQ Band to the mouse cursor
    /// </summary>
    /// <param name="e">The mouse event to listen on</param>
    /// <returns>The EQ band nearest the mouse cursor, if within range. Otherwise, returns nullptr</returns>
    const EQBand* getNearestEQBand(const juce::MouseEvent& e);
    /// <summary>
    /// Given a band's index and the final part of the parameter ID, returns the parameter for that band
    /// </summary>
    /// <param name="index">The index of the band</param>
    /// <param name="paramSuffix">The final part of the parameter ID. The parameter retrieved will be of the format "EQBAND[index]_[paramSuffix]"</param>
    /// <returns>A pointer to the RangedAudioParameter for the requested parameter</returns>
    juce::RangedAudioParameter* getBandParameter(const int index, const juce::String& paramSuffix);
    /// <summary>
    /// Updates a band's parameter with a specified value.
    /// </summary>
    /// <param name="index">The index of the band</param>
    /// <param name="paramSuffix">The final part of the parameter ID. The parameter retrieved will be of the format "EQBAND[index]_[paramSuffix]"</param>
    /// <param name="newValue">The new value to give the parameter</param>
    void updateBandParameter(const int index, const juce::String& paramSuffix, float newValue);

    void paint(juce::Graphics& g) override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;
    EqualizerModule& equalizer;

    const EQBand* currentDraggedBand = nullptr;

    void drawGridlines(juce::Graphics& g);

    /// <summary>
    /// Creates the filter selection submenu and adds it as an item to another menu
    /// </summary>
    /// <param name="menu">The menu to add the filter submenu to</param>
    void addFilterSelectionSubmenu(const EQBand* band, juce::PopupMenu& menu);

    void timerCallback() override;
};
