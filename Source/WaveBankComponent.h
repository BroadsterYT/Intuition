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


class WaveThumbnail : public juce::Component {
public:
    WaveThumbnail(int id, WavetableBank& bank);

    void mouseDown(const juce::MouseEvent& e) override;
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    WavetableBank& bank;
    int waveID = 0; // ID of the wave for this component to draw
};


class WaveBankComponent : public juce::Component {
public:
    WaveBankComponent(juce::AudioProcessor* ap, juce::AudioProcessorValueTreeState& vts, WavetableBank& bank);

    void resetProcessorSynths();
    void buildWaveComponents();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    IntuitionAudioProcessor* processor;
    juce::AudioProcessorValueTreeState& parameters;
    WavetableBank& bank;

    juce::TextButton addNewWaveButton, closeButton;
    juce::OwnedArray<WaveThumbnail> waveDiagrams;
    juce::Component waves;
    juce::Viewport viewport;
};