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
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    IntuitionAudioProcessor& audioProcessor;

    juce::Slider masterVolKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterVolAttachment;

    ADSRComponent adsrComponent;
    WaveformDisplay waveDisplay;
    OscillatorDisplay oscillatorDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IntuitionAudioProcessorEditor)
};
