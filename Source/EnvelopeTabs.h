/*
  ==============================================================================

    EnvelopeTabs.h
    Created: 24 Oct 2025 3:56:42pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include "EnvelopeDisplay.h"


class EnvelopeTabs : public juce::Component {
public:
    EnvelopeTabs(ItnContext& context);

    void resized() override;

private:
    ItnContext& context;
    juce::TabbedComponent tabs;

    EnvelopeDisplay envOsc;
    EnvelopeDisplay env1, env2, env3;
};
