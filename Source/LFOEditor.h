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


class LFOEditor : public juce::Component, private juce::Timer {
public:
    LFOEditor(LFOShape& shape, float* phase);
    ~LFOEditor();

    void paint(juce::Graphics& g) override;

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) override;

    LFOPoint* getNearestPoint(const juce::MouseEvent& e);

private:
    LFOShape& shape;
    float* phase = nullptr;

    void timerCallback() override;
};
