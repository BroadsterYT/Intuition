/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 22 Sep 2025 12:24:38am
    Author:  Brody

  ==============================================================================
*/

#include "WaveformDisplay.h"


WaveformDisplay::WaveformDisplay(
    juce::AudioProcessorValueTreeState& vts,
    ModMatrix* modMatrix,
    WavetableBank& bank,
    juce::String morphParamName
) : parameters(vts),
    modMatrix(modMatrix),
    bank(bank),
    morphParamName(morphParamName) {
    startTimerHz(30);
}

void WaveformDisplay::setBank(WavetableBank& newBank) {
    if (bank.size() == 0) return;
    bank = newBank;
    buildWaveform();
}

void WaveformDisplay::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);

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
    juce::ColourGradient grad = juce::ColourGradient::vertical(
        juce::Colours::lightgoldenrodyellow,
        0.0f,
        juce::Colours::darkgoldenrod,
        getHeight()
    );
    g.setGradientFill(grad);
    g.strokePath(path, juce::PathStrokeType(2.0f));
}

bool WaveformDisplay::needsRedraw() {
    float alpha = modMatrix->getModdedDest(morphParamName);

    bool result = false;
    if (std::abs(alpha - lastAlpha) >= 0.001f) {
        result = true;
    }

    lastAlpha = alpha;

    return result;
}

void WaveformDisplay::buildWaveform() {
    waveform.clear();

    float alpha = modMatrix->getModdedDest(morphParamName);
    float widx = alpha * (bank.size() - 1);
    int wi1 = (int)widx;
    float wFrac = widx - wi1;
    int wi2 = (wi1 + 1) % bank.size();

    auto* tableA = bank.getWavetable(wi1).getReadPointer(0);
    auto* tableB = bank.getWavetable(wi2).getReadPointer(0);

    int numSamples = bank.getWavetable(0).getNumSamples();
    waveform.ensureStorageAllocated(numSamples);

    for (int i = 0; i < numSamples; ++i) {
        waveform.add(((1 - wFrac) * tableA[i] + wFrac * tableB[i]));
    }
}

void WaveformDisplay::timerCallback() {
    if (needsRedraw()) {
        repaint();
    }
}