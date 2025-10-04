/*
  ==============================================================================

    LFOTabs.h
    Created: 4 Oct 2025 5:05:46pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "LFOEditor.h"
#include "LFOShape.h"


class LFOTabs : public juce::Component {
public:
    LFOTabs(LFOShape& shape1, LFOShape& shape2, LFOShape& shape3);

    void resized() override;

private:
    juce::TabbedComponent tabs;

    LFOShape& shape1, shape2, shape3;
    LFOEditor editor1, editor2, editor3;
};