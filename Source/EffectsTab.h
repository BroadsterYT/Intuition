/*
  ==============================================================================

    EffectsTab.h
    Created: 30 Oct 2025 5:26:31pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnLookAndFeel.h"
#include "ReverbDisplay.h"
#include "DelayDisplay.h"
#include "ItnContext.h"


class EffectsTab : public juce::Component {
public:
    EffectsTab(ItnContext& context);
    ~EffectsTab() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    ItnLookAndFeel lookAndFeel;
    
    ItnContext& context;

    ReverbDisplay reverb;
    DelayDisplay delay;
};