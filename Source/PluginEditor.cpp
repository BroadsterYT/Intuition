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
    mainTabs(p.context) {

    // =================================================
    addAndMakeVisible(mainTabs);
    addAndMakeVisible(midiKeyboard);
    midiKeyboard.setAvailableRange(48, 108);
    midiKeyboard.setScrollButtonsVisible(false);
    midiKeyboard.clearKeyMappings();

    masterVolKnob.setSliderStyle(juce::Slider::Rotary);
    masterVolKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    addAndMakeVisible(masterVolKnob);

    masterVolAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.parameters,
        "MASTER",
        masterVolKnob
    );

    setSize(1200, 800);
}

IntuitionAudioProcessorEditor::~IntuitionAudioProcessorEditor() {}

//==============================================================================
void IntuitionAudioProcessorEditor::paint (juce::Graphics& g) {
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void IntuitionAudioProcessorEditor::resized() {
    int width = getWidth();
    int height = getHeight();

    mainTabs.setBounds(getLocalBounds());
    masterVolKnob.setBounds(getWidth() - 100, 0, 100, 100);
    
    int kbPad = 120;
    int kbHeight = 80;
    int kbWidth = width - 2 * kbPad;
    int bottomPadding = 10;
    midiKeyboard.setKeyWidth(kbWidth / 36.0f);
    midiKeyboard.setBlackNoteWidthProportion(0.75);
    midiKeyboard.setBounds(kbPad, height - kbHeight - bottomPadding, kbWidth, kbHeight);
}
