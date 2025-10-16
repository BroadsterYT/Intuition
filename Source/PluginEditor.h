/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ADSRComponent.h"
#include "WaveformDisplay.h"
#include "OscillatorDisplay.h"
#include "LFOTabs.h"
#include "WaveBankComponent.h"


//==============================================================================
/**
*/
class IntuitionAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    IntuitionAudioProcessorEditor (IntuitionAudioProcessor&);
    ~IntuitionAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    IntuitionAudioProcessor& audioProcessor;
    juce::TooltipWindow tooltipWindow;
    juce::MidiKeyboardComponent midiKeyboard;

    juce::Slider masterVolKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterVolAttachment;

    ADSRComponent adsrComponent;
    
    OscillatorDisplay oscDisplay1;
    OscillatorDisplay oscDisplay2;
    OscillatorDisplay oscDisplay3;
    OscillatorDisplay oscDisplay4;
    
    LFOTabs lfoTabs;

    WaveBankComponent waveBankComp1;
    WaveBankComponent waveBankComp2;
    WaveBankComponent waveBankComp3;
    WaveBankComponent waveBankComp4;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IntuitionAudioProcessorEditor)
};
