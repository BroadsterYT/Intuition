/*
  ==============================================================================

    WaveBankComponent.h
    Created: 3 Oct 2025 9:11:09pm
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "WavetableBank.h"
#include "PluginProcessor.h"


class WaveDiagram : public juce::Component {
public:
    WaveDiagram(int id, WavetableBank& bank);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    WavetableBank& bank;
    int waveID = 0; // ID of the wave for this component to draw
};


class WaveBankComponent : public juce::Component {
public:
    WaveBankComponent(juce::AudioProcessor* ap, juce::AudioProcessorValueTreeState& vts, WavetableBank& bank);

    void buildWaveComponents();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    IntuitionAudioProcessor* processor;
    juce::AudioProcessorValueTreeState& parameters;
    WavetableBank& bank;

    juce::TextButton addNewWaveButton, closeButton;
    juce::OwnedArray<WaveDiagram> waveDiagrams;
    juce::Component waves;
    juce::Viewport viewport;
};