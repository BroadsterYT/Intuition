/*
  ==============================================================================

    LFOTabs.h
    Created: 4 Oct 2025 5:05:46pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "LFODisplay.h"
#include "LFOShape.h"
#include "ItnTabbedComponent.h"


class LFOTabs : public juce::Component {
public:
    LFOTabs(
        juce::AudioProcessorValueTreeState& vts,
        LFOShape& shape1,
        float* phase1,
        LFOShape& shape2,
        float* phase2,
        LFOShape& shape3,
        float* phase3
    );

    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    ItnTabbedComponent tabs;

    LFOShape& shape1, shape2, shape3;
    LFODisplay editor1, editor2, editor3;
};