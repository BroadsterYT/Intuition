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
#include "ModMatrix.h"
#include "WaveformDisplay.h"
#include "WavetableBank.h"
#include "WaveBankComponent.h"
#include "PanelTitleComponent.h"
#include "SliderComponent.h"


class OscillatorDisplay : public juce::Component, private juce::Timer {
public:
    OscillatorDisplay(
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
    );

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix = nullptr;

    juce::String toggleParamName;

    PanelTitleComponent title;

    SliderComponent volume, unison, detune, morph;
    SliderComponent octave, coarse, fine, pan;
    //ItnSlider unison, detune, morph;
    //ItnSlider octave, coarse, fine, pan;
    //std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> unisonAttachment, detuneAttachment, morphAttachment;
    //std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> octaveAttachment, coarseAttachment, fineAttachment, panAttachment;

    juce::TextButton waveBankEditorToggle;
    WaveBankComponent* waveBankComp;
    WaveformDisplay waveDisplay;

    void timerCallback() override;
};