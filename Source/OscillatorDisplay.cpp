/*
  ==============================================================================

    OscillatorDisplay.cpp
    Created: 24 Sep 2025 10:57:29pm
    Author:  Brody

  ==============================================================================
*/

#include "OscillatorDisplay.h"
#include "PluginProcessor.h"

OscillatorDisplay::OscillatorDisplay(juce::AudioProcessor* p, juce::AudioProcessorValueTreeState& vts, WavetableBank& bank) : parameters(vts), processor(p), waveDisplay(vts, bank) {
    addAndMakeVisible(waveDisplay);
    waveDisplay.setBank(bank);
    
    addAndMakeVisible(selectWaveButton);
    selectWaveButton.setButtonText("Edit");
    selectWaveButton.onClick = [this] () {
        auto chooser = std::make_shared<juce::FileChooser>(
            "Select a WAV file to open...",
            juce::File{},
            "*.wav"
        );

        chooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this, chooser] (const juce::FileChooser& fc) {
                auto file = fc.getResult();

                if (file.existsAsFile()) {
                    auto p = dynamic_cast<IntuitionAudioProcessor*>(processor);
                    p->addWavetableToBank1(file);
                    waveDisplay.setBank(p->bank1);
                }
            }
        );
    };
    
    addAndMakeVisible(unison);
    addAndMakeVisible(detune);
    addAndMakeVisible(morph);

    unisonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "A_UNISON", unison);
    detuneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "A_DETUNE", detune);
    morphAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "A_MORPH", morph);

    unison.setRange(1, 8, 1);
    detune.setRange(0, 100);
    morph.setRange(0.0f, 1.0f);

    unison.setLabelNames("Unison", "U");
    detune.setLabelNames("Detune", "D");
    morph.setLabelNames("Morph", "WTM");

    unison.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    detune.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    morph.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
}

void OscillatorDisplay::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::darkgrey);
    g.fillRect(getLocalBounds());
}

void OscillatorDisplay::resized() {
    auto area = getLocalBounds().reduced(10);

    auto knobArea = area.removeFromBottom(80);
    int knobWidth = knobArea.getWidth() / 3;
    unison.setBounds(knobArea.removeFromLeft(knobWidth));
    detune.setBounds(knobArea.removeFromLeft(knobWidth));
    morph.setBounds(knobArea.removeFromLeft(knobWidth));

    waveDisplay.setBounds(area);

    selectWaveButton.setBounds(area.getWidth() - 32, 0, 32, 32);
}