/*
  ==============================================================================

    EffectsTab.h
    Created: 30 Oct 2025 5:26:31pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ChorusDisplay.h"
#include "DelayDisplay.h"
#include "EqualizerDisplay.h"
#include "ItnContext.h"
#include "ItnLookAndFeel.h"
#include "ReverbDisplay.h"


class EffectsTab : public juce::Component {
public:
    EffectsTab(ItnContext& context);
    ~EffectsTab() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    ItnContext& context;

    ReverbDisplay reverb;
    DelayDisplay delay;
    ChorusDisplay chorus;
    EqualizerDisplay equalizer;
};