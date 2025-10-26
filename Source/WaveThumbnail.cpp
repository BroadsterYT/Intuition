/*
  ==============================================================================

    WaveThumbnail.cpp
    Created: 4 Oct 2025 4:50:47pm
    Author:  BroDe

  ==============================================================================
*/

#include "WaveThumbnail.h"
#include "WaveBankComponent.h"


WaveThumbnail::WaveThumbnail(int id, WavetableBank& bank) : bank(bank) {
    setLookAndFeel(&lookAndFeel);
    waveId = id;
}

WaveThumbnail::~WaveThumbnail() {
    setLookAndFeel(nullptr);
}

void WaveThumbnail::mouseDown(const juce::MouseEvent& e) {
    if (e.mods.isRightButtonDown()) {
        if (bank.size() > 1) {
            bank.removeWavetable(waveId);
        }
        if (auto* parent = findParentComponentOfClass<WaveBankComponent>()) {
            parent->buildWaveThumbnails();
            parent->resetProcessorSynths();
        }
    }
}

void WaveThumbnail::paint(juce::Graphics& g) {
    g.fillAll(GlowStyle::roomDark);

    int width = getWidth();
    int height = getHeight();

    buildReducedWaveform();
    lookAndFeel.drawWaveform(g, getBounds().toFloat(), waveform);
}

void WaveThumbnail::resized() {
    repaint();
}

void WaveThumbnail::buildReducedWaveform() {
    waveform.clear();

    int numSamples = bank.getWavetable(0).getNumSamples();
    waveform.ensureStorageAllocated(numSamples);

    auto* samples = bank.getWavetable(waveId).getReadPointer(0);
    for (int i = 0; i < numSamples; i += 2) {
        waveform.add(samples[i]);
    }
}
