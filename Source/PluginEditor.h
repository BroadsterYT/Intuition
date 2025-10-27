/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "EnvelopeDisplay.h"
#include "WaveformDisplay.h"
#include "OscillatorDisplay.h"
#include "LFOTabs.h"
#include "WaveBankComponent.h"
#include "FilterDisplay.h"
#include "MainTabs.h"
#include "ItnMidiKeyboard.h"


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
    ItnMidiKeyboard midiKeyboard;

    juce::Slider masterVolKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterVolAttachment;

    MainTabs mainTabs;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IntuitionAudioProcessorEditor)
};
