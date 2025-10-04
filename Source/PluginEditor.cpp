/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
IntuitionAudioProcessorEditor::IntuitionAudioProcessorEditor(IntuitionAudioProcessor& p
) : AudioProcessorEditor(&p),
    audioProcessor(p),
    tooltipWindow(this),
    midiKeyboard(p.keyboardState, juce::MidiKeyboardComponent::horizontalKeyboard),
    adsrComponent(p.parameters),
    
    oscDisplay1(
        p.parameters, 
        p.bank1, 
        &waveBankComp1, 
        "A_UNISON", 
        "A_DETUNE", 
        "A_MORPH",
        "A_OCTAVE",
        "A_COARSE",
        "A_FINE"
    ),

    lfoTabs(p.lfo1Shape, p.lfo2Shape, p.lfo3Shape),

    waveBankComp1(&p, p.parameters, p.bank1),
    waveBankComp2(&p, p.parameters, p.bank2),
    waveBankComp3(&p, p.parameters, p.bank3),
    waveBankComp4(&p, p.parameters, p.bank4) {
    // =================================================
    addAndMakeVisible(midiKeyboard);
    midiKeyboard.setAvailableRange(21, 108);

    masterVolKnob.setSliderStyle(juce::Slider::Rotary);
    masterVolKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(masterVolKnob);

    masterVolAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters,
        "MASTER",
        masterVolKnob
    );

    addAndMakeVisible(adsrComponent);

    addAndMakeVisible(oscDisplay1);
    
    addAndMakeVisible(lfoTabs);

    addChildComponent(waveBankComp1);

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
}

void IntuitionAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    masterVolKnob.setBounds(700, 0, 100, 100);
    midiKeyboard.setBounds(0, 550, 800, 50);

    int padding = 15;
    int adsrWidth = 250;
    int adsrHeight = 150;
    adsrComponent.setBounds(padding, padding, adsrWidth, adsrHeight);

    //waveDisplay.setBounds(adsrComponent.getRight() + padding, padding, 250, 150);

    oscDisplay1.setBounds(adsrComponent.getRight() + padding, padding, 250, 250);
    lfoTabs.setBounds(padding, adsrComponent.getBottom() + padding, 250, 150);

    waveBankComp1.setBounds(0, 0, 800, 600);
}
