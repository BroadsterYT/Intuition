/*
  ==============================================================================

    MainTabs.h
    Created: 23 Oct 2025 8:28:34pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnContext.h"
#include "ModMatrix.h"
#include "OscillatorTab.h"


/// <summary>
/// The component displaying all synthesizer components
/// </summary>
class MainTabs : public juce::Component, private juce::Timer {
public:
    MainTabs(ItnContext& context);
    
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    ItnContext& context;
    juce::TabbedComponent tabs;

    OscillatorTab oscTab;

    void timerCallback() override;
};