/*
  ==============================================================================

    OscillatorDisplay.cpp
    Created: 24 Sep 2025 10:57:29pm
    Author:  Brody

  ==============================================================================
*/

#include "OscillatorDisplay.h"


OscillatorDisplay::OscillatorDisplay(
    juce::AudioProcessorValueTreeState& vts,
    ModMatrix* modMatrix,
    WavetableBank& bank,
    WaveBankComponent* wbComp,
    
    const juce::String toggleParamName,
    const juce::String unisonParamName,
    const juce::String detuneParamName,
    const juce::String morphParamName,
    const juce::String octaveParamName,
    const juce::String coarseParamName,
    const juce::String fineParamName
) : parameters(vts),
    modMatrix(modMatrix),
    waveDisplay(vts, modMatrix, bank, morphParamName),
    waveBankComp(wbComp),
    
    toggleParamName(toggleParamName),
    unisonParamName(unisonParamName),
    detuneParamName(detuneParamName),
    morphParamName(morphParamName),
    octaveParamName(octaveParamName),
    coarseParamName(coarseParamName),
    fineParamName(fineParamName) {
    startTimerHz(60);

    addAndMakeVisible(waveDisplay);
    waveDisplay.setBank(bank);
    
    addAndMakeVisible(waveBankEditorToggle);
    waveBankEditorToggle.setButtonText("Edit");

    waveBankEditorToggle.onClick = [this] {
        waveBankComp->setVisible(true);
    };
    
    toggleAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(parameters, toggleParamName, toggle);
    unisonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, unisonParamName, unison);
    detuneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, detuneParamName, detune);
    morphAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, morphParamName, morph);
    octaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, octaveParamName, octave);
    coarseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, coarseParamName, coarse);
    fineAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, fineParamName, fine);

    //unison.setModMatrix(modMatrix, unisonParamName);
    detune.setModMatrix(modMatrix, detuneParamName);
    morph.setModMatrix(modMatrix, morphParamName);
    octave.setModMatrix(modMatrix, octaveParamName);
    coarse.setModMatrix(modMatrix, coarseParamName);
    fine.setModMatrix(modMatrix, fineParamName);

    unison.setRange(1, 8, 1);
    detune.setRange(0, 100);
    morph.setRange(0.0f, 1.0f);
    octave.setRange(-4, 4, 1);
    coarse.setRange(-12, 12, 1);
    fine.setRange(-100, 100, 1);

    toggle.setButtonText("");
    unison.setLabelNames("Unison", "U");
    detune.setLabelNames("Detune", "D");
    morph.setLabelNames("Morph", "WTM");
    octave.setLabelNames("Octave", "OCT");
    coarse.setLabelNames("Coarse Pitch", "SEM");
    fine.setLabelNames("Fine Pitch", "FIN");

    unison.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    detune.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    morph.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    octave.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    coarse.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    fine.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);

    addAndMakeVisible(toggle);
    addAndMakeVisible(unison);
    addAndMakeVisible(detune);
    addAndMakeVisible(morph);
    addAndMakeVisible(octave);
    addAndMakeVisible(coarse);
    addAndMakeVisible(fine);
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

    auto pitchArea = area.removeFromBottom(60);
    int pitchWidth = pitchArea.getWidth() / 3;
    octave.setBounds(pitchArea.removeFromLeft(pitchWidth));
    coarse.setBounds(pitchArea.removeFromLeft(pitchWidth));
    fine.setBounds(pitchArea.removeFromLeft(pitchWidth));

    toggle.setBounds(10, 10, 25, 25);

    waveDisplay.setBounds(area);
    waveBankEditorToggle.setBounds(area.getWidth() - 32, 10, 40, 24);
}

void OscillatorDisplay::timerCallback() {
    repaint();
}
