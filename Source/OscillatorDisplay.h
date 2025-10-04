/*
  ==============================================================================

    OscillatorDisplay.h
    Created: 24 Sep 2025 10:57:20pm
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnSlider.h"
#include "WaveformDisplay.h"
#include "WavetableBank.h"
#include "WaveBankComponent.h"


class OscillatorDisplay : public juce::Component {
public:
    OscillatorDisplay(juce::AudioProcessorValueTreeState& vts, WavetableBank& bank, WaveBankComponent* wbComp);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::AudioProcessor* processor;
    juce::AudioProcessorValueTreeState& parameters;

    ItnSlider unison, detune, morph;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> unisonAttachment, detuneAttachment, morphAttachment;

    juce::TextButton waveBankEditorToggle;
    WaveBankComponent* waveBankComp;
    WaveformDisplay waveDisplay;
};