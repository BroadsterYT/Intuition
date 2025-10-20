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
        &p.modMatrix,
        p.bank1, 
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
        p.parameters,
        &p.modMatrix,
        p.bank2,
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
        p.parameters,
        &p.modMatrix,
        p.bank3,
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
        p.parameters,
        &p.modMatrix,
        p.bank4,
        &waveBankComp4,
        "D_TOGGLE",
        "D_UNISON",
        "D_DETUNE",
        "D_MORPH",
        "D_OCTAVE",
        "D_COARSE",
        "D_FINE"
    ),

    lfoTabs(
        p.parameters,
        p.lfoShape1, p.getLFOPhase(1),
        p.lfoShape2, p.getLFOPhase(2),
        p.lfoShape3, p.getLFOPhase(3)
    ),
    filterDisplay(p.parameters, &p.modMatrix),

    waveBankComp1(&p, p.parameters, p.bank1),
    waveBankComp2(&p, p.parameters, p.bank2),
    waveBankComp3(&p, p.parameters, p.bank3),
    waveBankComp4(&p, p.parameters, p.bank4) {
    // =================================================
    addAndMakeVisible(midiKeyboard);
    midiKeyboard.setAvailableRange(21, 108);
    midiKeyboard.clearKeyMappings();

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
    addAndMakeVisible(oscDisplay2);
    addAndMakeVisible(oscDisplay3);
    addAndMakeVisible(oscDisplay4);
    
    addAndMakeVisible(lfoTabs);
    addAndMakeVisible(filterDisplay);

    addChildComponent(waveBankComp1);
    addChildComponent(waveBankComp2);
    addChildComponent(waveBankComp3);
    addChildComponent(waveBankComp4);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(1200, 800);
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
    masterVolKnob.setBounds(getWidth() - 100, 0, 100, 100);
    midiKeyboard.setBounds(0, 550, 800, 50);

    int padding = 15;
    int adsrWidth = 250;
    int adsrHeight = 150;
    adsrComponent.setBounds(padding, padding, adsrWidth, adsrHeight);

    //waveDisplay.setBounds(adsrComponent.getRight() + padding, padding, 250, 150);

    oscDisplay1.setBounds(adsrComponent.getRight() + padding, padding, 200, 250);
    oscDisplay2.setBounds(oscDisplay1.getRight() + padding, padding, 200, 250);
    oscDisplay3.setBounds(adsrComponent.getRight() + padding, oscDisplay1.getBottom() + padding, 200, 250);
    oscDisplay4.setBounds(oscDisplay3.getRight() + padding, oscDisplay1.getBottom() + padding, 200, 250);
    lfoTabs.setBounds(padding, adsrComponent.getBottom() + padding, 250, 250);
    filterDisplay.setBounds(oscDisplay4.getRight() + padding, padding, 250, 250);

    waveBankComp1.setBounds(0, 0, 800, 600);
    waveBankComp2.setBounds(0, 0, 800, 600);
    waveBankComp3.setBounds(0, 0, 800, 600);
    waveBankComp4.setBounds(0, 0, 800, 600);
}
