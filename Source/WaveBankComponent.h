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
#include "WaveThumbnail.h"


class WaveBankComponent : public juce::Component {
public:
    WaveBankComponent(
        juce::AudioProcessor* ap,
        juce::AudioProcessorValueTreeState& vts,
        WavetableBank& bank
    );

    void resetProcessorSynths();
    void buildWaveThumbnails();

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    IntuitionAudioProcessor* processor;
    juce::AudioProcessorValueTreeState& parameters;
    WavetableBank& bank;

    juce::TextButton addNewWaveButton, closeButton;
    juce::OwnedArray<WaveThumbnail> waveThumbnails;
    juce::Component waveThumbnailComp;
    juce::Viewport viewport;
};