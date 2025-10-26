/*
  ==============================================================================

    LFOEditor.h
    Created: 26 Sep 2025 12:36:35am
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "LFOShape.h"
#include "InlineValueEntry.h"
#include "ItnLookAndFeel.h"


class LFOEditor : public juce::Component, private juce::Timer {
public:
    LFOEditor(LFOShape& shape, float* phase);
    ~LFOEditor();

    void paint(juce::Graphics& g) override;

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override;

    LFOPoint* getNearestPoint(const juce::MouseEvent& e);

    void visibilityChanged() override;

private:
    ItnLookAndFeel lookAndFeel;

    LFOShape& shape;
    float* phase = nullptr;

    /// <summary>
    /// Draw the LFO shape and returns it as a path
    /// </summary>
    /// <returns>The path object to draw onto the component</returns>
    juce::Path drawLFOPath();
    /// <summary>
    /// Draws the LFO points directly onto the component
    /// </summary>
    /// <param name="g">The JUCE graphics object</param>
    void drawLFOPoints(juce::Graphics& g);
    void timerCallback() override;
};
