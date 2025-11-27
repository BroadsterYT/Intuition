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
    unisonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, unisonParamName, unison);
    detuneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, detuneParamName, detune);
    morphAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, morphParamName, morph);
    octaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, octaveParamName, octave);
    coarseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, coarseParamName, coarse);
    fineAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, fineParamName, fine);

    //unison.setModMatrix(modMatrix, unisonParamName);
    volume.setModMatrix(modMatrix, volumeParamName);
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
    volume.setLabelNames("Volume", "Vol");
    unison.setLabelNames("Unison", "U");
    detune.setLabelNames("Detune", "D");
    morph.setLabelNames("Morph", "WTM");
    octave.setLabelNames("Octave", "OCT");
    coarse.setLabelNames("Coarse Pitch", "SEM");
    fine.setLabelNames("Fine Pitch", "FIN");

    volume.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    unison.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    detune.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    morph.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    octave.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    coarse.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);
    fine.setTextBoxStyle(juce::Slider::NoTextBox, false, 1, 1);

    addAndMakeVisible(toggle);
    addAndMakeVisible(volume);
    addAndMakeVisible(unison);
    addAndMakeVisible(detune);
    addAndMakeVisible(morph);
    addAndMakeVisible(octave);
    addAndMakeVisible(coarse);
    addAndMakeVisible(fine);

    volume.setTooltipFields(
        "Volume",
        "Turn this one down if you hate fun.",
        "Controls the loudness of your sound. A dynamic loudness can completely change the feeling of your sound for the better (or worse)."
    );
    unison.setTooltipFields(
        "Unison",
        "Stack 'em high, stack 'em loud.",
        "Makes this oscillator play multiple copies of its wave, stacking them together for a richer, thicker tone. Use this to fatten leads, pads, or basses."
    );
    detune.setTooltipFields(
        "Unison Detune",
        "Detune responsibly... or don't.",
        "Slightly shifts the pitch of each unison away from the base, creating a thicker, gnarlier sound. The more you detune, the wilder the chorus effect."
    );
    morph.setTooltipFields(
        "Wavetable Morph",
        "Look, Serum! No hands!",
        "Shifts smoothly between the waveforms in this oscillator's wavebank. Use this to create evolving textures, dynamic timbres, or sounds that just keep changing."
    );
    octave.setTooltipFields(
        "Octave",
        "From basement rumble to dog whistle.",
        "Shifts the pitch of this oscillator up or down in steps of whole octaves. A step up doubles the frequency of the sound, while a step down halves it. Use this to quickly jump between bass, mid-range, and treble."
    );
    coarse.setTooltipFields(
        "Coarse Pitch (Semitones)",
        "Adds grit.",
        "Shifts the pitch of this oscillator up or down in single-note steps, called semitones. Each step raises or lowers the pitch by 1/12th of an octave. For example, playing middle C (C4) with this dial set to -1 will actually sound like B4."
    );
    fine.setTooltipFields(
        "Fine Pitch (Cents)",
        "Vibrato's best friend.",
        "Shifts the pitch of this oscillator up or down in fractions of a semitone, called cents. Increasing/decreasing pitch by 100 cents is the same as increasing/decreasing by 1 semitone."
    );
}

void OscillatorDisplay::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::darkgrey);
    g.fillRect(getLocalBounds());
}

void OscillatorDisplay::resized() {
    auto area = getLocalBounds().reduced(10);

    auto knobArea = area.removeFromBottom(80);
    int knobWidth = knobArea.getWidth() / 4;
    unison.setBounds(knobArea.removeFromLeft(knobWidth));
    detune.setBounds(knobArea.removeFromLeft(knobWidth));
    morph.setBounds(knobArea.removeFromLeft(knobWidth));
    volume.setBounds(knobArea.removeFromLeft(knobWidth));

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
