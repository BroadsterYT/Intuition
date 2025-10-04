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
    waveId = id;
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
    g.fillAll(juce::Colours::black);

    int width = getWidth();
    int height = getHeight();

    juce::Array<float> waveform;
    int numSamples = bank.getWavetable(0).getNumSamples();
    waveform.ensureStorageAllocated(numSamples);

    auto* samples = bank.getWavetable(waveId).getReadPointer(0);
    for (int i = 0; i < numSamples; ++i) {
        waveform.add(samples[i]);
    }

    juce::Path path;
    path.startNewSubPath(0, (float)height / 2);

    for (int i = 0; i < waveform.size(); ++i) {
        float x = juce::jmap((float)i, 0.0f, (float)waveform.size(), 0.0f, (float)width);
        float y = juce::jmap(waveform[i], -1.0f, 1.0f, (float)height, 0.0f);
        path.lineTo(x, y);
    }

    g.setColour(juce::Colours::aquamarine);
    g.strokePath(path, juce::PathStrokeType(2.0f));
}

void WaveThumbnail::resized() {
    repaint();
}
