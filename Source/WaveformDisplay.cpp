/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 22 Sep 2025 12:24:38am
    Author:  Brody

  ==============================================================================
*/

#include "WaveformDisplay.h"


WaveformDisplay::WaveformDisplay() {}

void WaveformDisplay::setWaveform(const juce::AudioBuffer<float>& buffer) {
    if (buffer.getNumSamples() == 0) return;

    waveform.clear();
    auto* samples = buffer.getReadPointer(0);

    int numSamples = buffer.getNumSamples();
    waveform.ensureStorageAllocated(numSamples);

    for (int i = 0; i < numSamples; ++i) {
        waveform.add(samples[i]);
    }

    repaint();
}

void WaveformDisplay::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);

    if (waveform.size() == 0) return;

    int width = getWidth();
    int height = getHeight();

    juce::Path path;
    path.startNewSubPath(0, height / 2);

    for (int i = 0; i < waveform.size(); ++i) {
        float x = juce::jmap((float)i, 0.0f, (float)waveform.size(), 0.0f, (float)width);
        float y = juce::jmap(waveform[i], -1.0f, 1.0f, (float)height, 0.0f);
        path.lineTo(x, y);
    }

    g.strokePath(path, juce::PathStrokeType(2.0f));
}