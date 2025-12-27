/*
  ==============================================================================

    EnvelopeTabs.h
    Created: 24 Oct 2025 3:56:42pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include "EnvelopeDisplay.h"
#include "ItnTabbedComponent.h"


class EnvelopeTabs : public juce::Component {
public:
    EnvelopeTabs(ItnContext& context);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    ItnContext& context;
    ItnTabbedComponent tabs;

    EnvelopeDisplay envOsc;
    EnvelopeDisplay env1, env2, env3;
};
