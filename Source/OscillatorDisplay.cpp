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
    
    volume(vts, volumeParamName, "Volume"),
    pan(vts, panParamName, "Pan"),
    unison(vts, unisonParamName, "Unison"),
    detune(vts, detuneParamName, "Detune", "ct"),
    morph(vts, morphParamName, "WTM"),
    octave(vts, octaveParamName, "Octave", "oct"),
    coarse(vts, coarseParamName, "Coarse", "sem"),
    fine(vts, fineParamName, "Fine", "ct"),

    title(vts, "Wavetable", true, toggleParamName) {
    startTimerHz(60);
    addAndMakeVisible(title);

    addAndMakeVisible(waveDisplay);
    waveDisplay.setBank(bank);

    addAndMakeVisible(waveBankEditorToggle);
    waveBankEditorToggle.setButtonText("Edit");

    waveBankEditorToggle.onClick = [this] {
        waveBankComp->setVisible(true);
    };

    volume.setModMatrix(modMatrix);
    pan.setModMatrix(modMatrix);
    detune.setModMatrix(modMatrix);
    morph.setModMatrix(modMatrix);
    octave.setModMatrix(modMatrix);
    coarse.setModMatrix(modMatrix);
    fine.setModMatrix(modMatrix);

    unison.setRange(1, 8, 1);
    detune.setRange(0, 100);
    morph.setRange(0.0f, 1.0f);
    octave.setRange(-4, 4, 1);
    coarse.setRange(-12, 12, 1);
    fine.setRange(-100, 100, 1);

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
    auto titleArea = area.removeFromTop(33);
    area = area.reduced(10);

    title.setBounds(titleArea);

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

    /*titleArea.removeFromLeft(5);
    toggle.setBounds(titleArea.removeFromLeft(25));
    title.setBounds(toggle.getRight(), 5, 100, 25);*/

    waveDisplay.setBounds(area);
    waveBankEditorToggle.setBounds(area.getWidth() - 32, 10, 40, 24);
}

void OscillatorDisplay::timerCallback() {
    repaint();
}
