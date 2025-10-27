/*
  ==============================================================================

    OscillatorTab.h
    Created: 23 Oct 2025 9:03:44pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include "EnvelopeTabs.h"
#include "ItnContext.h"
#include "OscillatorDisplay.h"
#include "LFOTabs.h"
#include "FilterDisplay.h"
#include "ItnLookAndFeel.h"


/// <summary>
/// The main tab seen when opening the synth. Contains oscillators,
/// envelopes, filters, and lfos
/// </summary>
class OscillatorTab : public juce::Component {
public:
    OscillatorTab(ItnContext& context);
    ~OscillatorTab() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    ItnLookAndFeel lookAndFeel;
    ItnContext& context;

    WaveBankComponent waveBankComp1;
    WaveBankComponent waveBankComp2;
    WaveBankComponent waveBankComp3;
    WaveBankComponent waveBankComp4;

    OscillatorDisplay oscDisplay1;
    OscillatorDisplay oscDisplay2;
    OscillatorDisplay oscDisplay3;
    OscillatorDisplay oscDisplay4;

    EnvelopeTabs envTabs;
    LFOTabs lfoTabs;

    FilterDisplay filterDisplay;
};