/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
IntuitionAudioProcessorEditor::IntuitionAudioProcessorEditor (IntuitionAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), adsrComponent(audioProcessor.parameters),
    oscillatorDisplay(audioProcessor.parameters)
{
    masterVolKnob.setSliderStyle(juce::Slider::Rotary);
    masterVolKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(masterVolKnob);

    masterVolAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters,
        "MASTER",
        masterVolKnob
    );

    addAndMakeVisible(adsrComponent);

    addAndMakeVisible(waveDisplay);
    waveDisplay.setWaveform(audioProcessor.wavetableBuffer);

    addAndMakeVisible(oscillatorDisplay);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(800, 600);
}

IntuitionAudioProcessorEditor::~IntuitionAudioProcessorEditor()
{
}

//==============================================================================
void IntuitionAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    /*g.setColour (juce::Colours::black);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);*/
}

void IntuitionAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    masterVolKnob.setBounds(700, 0, 100, 100);

    int padding = 15;
    int adsrWidth = 250;
    int adsrHeight = 150;
    adsrComponent.setBounds(padding, padding, adsrWidth, adsrHeight);

    waveDisplay.setBounds(adsrComponent.getRight() + padding, padding, 250, 150);

    oscillatorDisplay.setBounds(adsrComponent.getRight() + padding, waveDisplay.getBottom() + padding, 250, 100);
}
