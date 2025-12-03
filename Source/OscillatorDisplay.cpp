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
    const juce::String volumeParamName,
    const juce::String panParamName,
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
    volumeParamName(volumeParamName),
    panParamName(panParamName),
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
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, volumeParamName, volume);
    panAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, panParamName, pan);
    unisonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, unisonParamName, unison);
    detuneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, detuneParamName, detune);
    morphAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, morphParamName, morph);
    octaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, octaveParamName, octave);
    coarseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, coarseParamName, coarse);
    fineAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, fineParamName, fine);

    volume.setModMatrix(modMatrix, volumeParamName);
    pan.setModMatrix(modMatrix, panParamName);
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
    volume.setLabelName("Vol");
    pan.setLabelName("Pan");
    unison.setLabelName("U");
    detune.setLabelName("D");
    morph.setLabelName("WTM");
    octave.setLabelName("OCT");
    coarse.setLabelName("SEM");
    fine.setLabelName("FIN");

    volume.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    pan.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    unison.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    detune.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    morph.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    octave.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    coarse.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    fine.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);

    title.setText("Wavebank", juce::dontSendNotification);

    addAndMakeVisible(title);
    addAndMakeVisible(toggle);
    addAndMakeVisible(volume);
    addAndMakeVisible(pan);
    addAndMakeVisible(unison);
    addAndMakeVisible(detune);
    addAndMakeVisible(morph);
    addAndMakeVisible(octave);
    addAndMakeVisible(coarse);
    addAndMakeVisible(fine);

    volume.setCustomTooltipText("OSC_VOLUME");
    pan.setCustomTooltipText("OSC_PAN");
    unison.setCustomTooltipText("OSC_UNISON");
    detune.setCustomTooltipText("OSC_DETUNE");
    morph.setCustomTooltipText("OSC_MORPH");
    octave.setCustomTooltipText("OSC_OCTAVE");
    coarse.setCustomTooltipText("OSC_COARSE");
    fine.setCustomTooltipText("OSC_FINE");
}

void OscillatorDisplay::paint(juce::Graphics& g) {
    ItnLookAndFeel::drawComponentPanel(g, getLocalBounds().toFloat());
}

void OscillatorDisplay::resized() {
    auto area = getLocalBounds();
    auto titleArea = area.removeFromTop(32);
    area = area.reduced(10);

    auto knobArea = area.removeFromBottom(80);
    int knobWidth = knobArea.getWidth() / 4;
    unison.setBounds(knobArea.removeFromLeft(knobWidth));
    detune.setBounds(knobArea.removeFromLeft(knobWidth));
    morph.setBounds(knobArea.removeFromLeft(knobWidth));
    volume.setBounds(knobArea.removeFromLeft(knobWidth));

    auto pitchArea = area.removeFromBottom(60);
    int pitchWidth = pitchArea.getWidth() / 4;
    octave.setBounds(pitchArea.removeFromLeft(pitchWidth));
    coarse.setBounds(pitchArea.removeFromLeft(pitchWidth));
    fine.setBounds(pitchArea.removeFromLeft(pitchWidth));
    pan.setBounds(pitchArea.removeFromLeft(pitchWidth));

    titleArea.removeFromLeft(5);
    toggle.setBounds(titleArea.removeFromLeft(25));
    title.setBounds(toggle.getRight(), 5, 100, 25);

    waveDisplay.setBounds(area);
    waveBankEditorToggle.setBounds(area.getWidth() - 32, 10, 40, 24);
}

void OscillatorDisplay::timerCallback() {
    repaint();
}
