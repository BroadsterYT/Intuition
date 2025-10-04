/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 22 Sep 2025 12:24:38am
    Author:  Brody

  ==============================================================================
*/

#include "WaveformDisplay.h"


WaveformDisplay::WaveformDisplay(juce::AudioProcessorValueTreeState& vts, WavetableBank& bank) : parameters(vts), bank(bank) {
    startTimerHz(60);
}

void WaveformDisplay::setBank(WavetableBank& bank) {
    if (bank.size() == 0) return;
    this->bank = bank;

    buildWaveform();
}

void WaveformDisplay::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);

    if (waveform.size() == 0) return;

    int width = getWidth();
    int height = getHeight();

    buildWaveform();

    juce::Path path;
    path.startNewSubPath(0, height / 2);

    for (int i = 0; i < waveform.size(); ++i) {
        float x = juce::jmap((float)i, 0.0f, (float)waveform.size(), 0.0f, (float)width);
        float y = juce::jmap(waveform[i], -1.0f, 1.0f, (float)height, 0.0f);
        path.lineTo(x, y);
    }

    g.strokePath(path, juce::PathStrokeType(2.0f));
}

void WaveformDisplay::buildWaveform() {
    waveform.clear();
    float alpha = *parameters.getRawParameterValue("A_MORPH");
    auto* samplesA = bank.getWavetable(0).getReadPointer(0);
    auto* samplesB = bank.getWavetable(bank.size() - 1).getReadPointer(0);

    int numSamples = bank.getWavetable(0).getNumSamples();
    waveform.ensureStorageAllocated(numSamples);

    // TODO: Draw morphed wave
    for (int i = 0; i < numSamples; ++i) {
        waveform.add(((1 - alpha) * samplesA[i] + alpha * samplesB[i]));
    }
}

void WaveformDisplay::timerCallback() {
    repaint();
}