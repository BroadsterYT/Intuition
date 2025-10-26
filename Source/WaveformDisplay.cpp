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
    setLookAndFeel(&lookAndFeel);
    startTimerHz(30);
}

WaveformDisplay::~WaveformDisplay() {
    setLookAndFeel(nullptr);
}

void WaveformDisplay::setBank(WavetableBank& newBank) {
    if (bank.size() == 0) return;
    bank = newBank;
    buildWaveform();
}

void WaveformDisplay::paint(juce::Graphics& g) {
    g.fillAll(GlowStyle::roomDark);
    buildWaveform();
    lookAndFeel.drawWaveform(g, getBounds().toFloat(), waveform);
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