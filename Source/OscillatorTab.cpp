/*
  ==============================================================================

    OscillatorTab.cpp
    Created: 23 Oct 2025 9:03:44pm
    Author:  BroDe

  ==============================================================================
*/

#include "OscillatorTab.h"

OscillatorTab::OscillatorTab(ItnContext& context
) : context(context),
    waveBankComp1(
        context.ap,
        context.parameters,
        context.bank1
    ),
    waveBankComp2(
        context.ap,
        context.parameters,
        context.bank2
    ),
    waveBankComp3(
        context.ap,
        context.parameters,
        context.bank3
    ),
    waveBankComp4(
        context.ap,
        context.parameters,
        context.bank4
    ),

    oscDisplay1(
        context.parameters,
        context.modMatrix,
        context.bank1,
        &waveBankComp1,
        "A_TOGGLE",
        "A_UNISON",
        "A_DETUNE",
        "A_MORPH",
        "A_OCTAVE",
        "A_COARSE",
        "A_FINE"
    ),
    oscDisplay2(
        context.parameters,
        context.modMatrix,
        context.bank2,
        &waveBankComp2,
        "B_TOGGLE",
        "B_UNISON",
        "B_DETUNE",
        "B_MORPH",
        "B_OCTAVE",
        "B_COARSE",
        "B_FINE"
    ),
    oscDisplay3(
        context.parameters,
        context.modMatrix,
        context.bank3,
        &waveBankComp3,
        "C_TOGGLE",
        "C_UNISON",
        "C_DETUNE",
        "C_MORPH",
        "C_OCTAVE",
        "C_COARSE",
        "C_FINE"
    ),
    oscDisplay4(
        context.parameters,
        context.modMatrix,
        context.bank4,
        &waveBankComp4,
        "D_TOGGLE",
        "D_UNISON",
        "D_DETUNE",
        "D_MORPH",
        "D_OCTAVE",
        "D_COARSE",
        "D_FINE"
    ),

    envTabs(context.parameters),
    lfoTabs(
        context.parameters,
        context.shape1,
        context.phase1,
        context.shape2,
        context.phase2,
        context.shape3,
        context.phase3
    ),

    filterDisplay(
        context.parameters,
        context.modMatrix
    ) {
    addAndMakeVisible(oscDisplay1);
    addAndMakeVisible(oscDisplay2);
    addAndMakeVisible(oscDisplay3);
    addAndMakeVisible(oscDisplay4);
    
    addAndMakeVisible(envTabs);
    addAndMakeVisible(lfoTabs);
    addAndMakeVisible(filterDisplay);

    addChildComponent(waveBankComp1);
    addChildComponent(waveBankComp2);
    addChildComponent(waveBankComp3);
    addChildComponent(waveBankComp4);
}

void OscillatorTab::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::dimgrey);
}

void OscillatorTab::resized() {
    int padding = 15;
    int adsrWidth = 250;
    int adsrHeight = 150;
    envTabs.setBounds(padding, padding, adsrWidth, adsrHeight);

    oscDisplay1.setBounds(envTabs.getRight() + padding, padding, 200, 250);
    oscDisplay2.setBounds(oscDisplay1.getRight() + padding, padding, 200, 250);
    oscDisplay3.setBounds(envTabs.getRight() + padding, oscDisplay1.getBottom() + padding, 200, 250);
    oscDisplay4.setBounds(oscDisplay3.getRight() + padding, oscDisplay1.getBottom() + padding, 200, 250);
    lfoTabs.setBounds(padding, envTabs.getBottom() + padding, 250, 250);
    filterDisplay.setBounds(oscDisplay4.getRight() + padding, padding, 250, 250);

    waveBankComp1.setBounds(getLocalBounds());
    waveBankComp2.setBounds(getLocalBounds());
    waveBankComp3.setBounds(getLocalBounds());
    waveBankComp4.setBounds(getLocalBounds());
}
