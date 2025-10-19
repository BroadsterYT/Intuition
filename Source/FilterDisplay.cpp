/*
  ==============================================================================

    FilterDisplay.cpp
    Created: 19 Oct 2025 3:36:37pm
    Author:  BroDe

  ==============================================================================
*/

#include "FilterDisplay.h"

FilterDisplay::FilterDisplay(
    juce::AudioProcessorValueTreeState& vts,
    ModMatrix* modMatrix
) : parameters(vts),
    modMatrix(modMatrix),
    graph(vts) {

    frequencyAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "FILTER_CUTOFF", frequency);
    resonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, "FILTER_RESONANCE", resonance);
    typeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(parameters, "FILTER_TYPE", type);

    type.addItem("Lowpass", 1);
    type.addItem("Highpass", 2);
    type.addItem("Bandpass", 3);
    type.setSelectedId(1);

    frequency.setRange(20.0f, 20000.0f);
    resonance.setRange(0.01f, 1.0f);

    frequency.setSkewFactorFromMidPoint(1000.0f);

    frequency.setLabelNames("Cutoff", "Cutoff");
    resonance.setLabelNames("Resonance", "Res");

    frequency.setModMatrix(modMatrix, "FILTER_CUTOFF");
    resonance.setModMatrix(modMatrix, "FILTER_RESONANCE");

    addAndMakeVisible(graph);
    addAndMakeVisible(frequency);
    addAndMakeVisible(resonance);
    addAndMakeVisible(type);

    aToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(parameters, "A_FILTER_SEND", toggleA);
    bToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(parameters, "B_FILTER_SEND", toggleB);
    cToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(parameters, "C_FILTER_SEND", toggleC);
    dToggleAttach = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(parameters, "D_FILTER_SEND", toggleD);

    toggleA.setButtonText("A");
    toggleB.setButtonText("B");
    toggleC.setButtonText("C");
    toggleD.setButtonText("D");

    addAndMakeVisible(toggleA);
    addAndMakeVisible(toggleB);
    addAndMakeVisible(toggleC);
    addAndMakeVisible(toggleD);
}

void FilterDisplay::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::darkgrey);
    g.fillRect(getLocalBounds());
}

void FilterDisplay::resized() {
    auto area = getLocalBounds();

    auto filterTypeArea = area.removeFromTop(40);
    type.setBounds(filterTypeArea.reduced(10));

    auto dialArea = area.removeFromBottom(100).reduced(10);
    auto sendArea = dialArea.removeFromLeft(50);

    int toggleHeight = sendArea.getHeight() / 4;
    toggleA.setBounds(sendArea.removeFromTop(toggleHeight));
    toggleB.setBounds(sendArea.removeFromTop(toggleHeight));
    toggleC.setBounds(sendArea.removeFromTop(toggleHeight));
    toggleD.setBounds(sendArea.removeFromTop(toggleHeight));

    int knobWidth = dialArea.getWidth() / 2;
    frequency.setBounds(dialArea.removeFromLeft(knobWidth));
    resonance.setBounds(dialArea.removeFromLeft(knobWidth));

    graph.setBounds(area.reduced(10));
}
