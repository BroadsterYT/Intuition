/*
  ==============================================================================

    EnvelopeGraph.h
    Created: 24 Oct 2025 1:26:21pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnContext.h"


/// <summary>
/// A component displaying an ADSR envelope
/// </summary>
class EnvelopeGraph : public juce::Component {
public:
    EnvelopeGraph(ItnContext& context);

    void paint(juce::Graphics& g) override;

private:
    ItnContext& context;
};