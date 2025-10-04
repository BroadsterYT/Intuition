/*
  ==============================================================================

    WaveBankComponent.cpp
    Created: 3 Oct 2025 9:11:09pm
    Author:  Brody

  ==============================================================================
*/

#include "WaveBankComponent.h"


WaveBankComponent::WaveBankComponent(juce::AudioProcessor* ap, juce::AudioProcessorValueTreeState& vts, WavetableBank& bank) : parameters(vts), bank(bank) {
    processor = dynamic_cast<IntuitionAudioProcessor*>(ap);
    
    addAndMakeVisible(viewport);
    viewport.setViewedComponent(&waves, false);

    addAndMakeVisible(closeButton);
    closeButton.onClick = [this] {
        setVisible(false);
    };

    waves.addAndMakeVisible(addNewWaveButton);
    addNewWaveButton.setButtonText("Add New");
    addNewWaveButton.onClick = [this, &bank] () {
        auto chooser = std::make_shared<juce::FileChooser>(
            "Select a WAV file to open...",
            juce::File{},
            "*.wav"
        );

        chooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this, chooser, &bank] (const juce::FileChooser& fc) {
                auto file = fc.getResult();

                processor->addWavetableToBank1(file);
                buildWaveComponents();
            }
        );
    };
}

void WaveBankComponent::buildWaveComponents() {
    waveDiagrams.clear(true);

    int i = 0;
    for (i = 0; i < bank.size(); ++i) {
        auto* newWave = new WaveDiagram(i, bank);
        newWave->setBounds(15 * (i + 1) + 80 * i, 15, 80, 80);
        waves.addAndMakeVisible(newWave);
        waveDiagrams.add(newWave);
    }

    addNewWaveButton.setBounds(15 * (i + 1) + 80 * i, waves.getHeight() / 2 - 24, 80, 32);
}

void WaveBankComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::darkgrey);
    g.fillRect(getLocalBounds());
}

void WaveBankComponent::resized() {
    auto area = getLocalBounds();

    viewport.setBounds(area.removeFromBottom(128).reduced(10));
    waves.setBounds(0, 0, 1600, 80 + 15);
    closeButton.setBounds(getWidth() - 32 - 16, 32, 32, 32);

    buildWaveComponents();
}


WaveDiagram::WaveDiagram(int id, WavetableBank& bank) : bank(bank) {
    waveID = id;
}

void WaveDiagram::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    int width = getWidth();
    int height = getHeight();
    
    juce::Array<float> waveform;
    int numSamples = bank.getWavetable(0).getNumSamples();
    waveform.ensureStorageAllocated(numSamples);

    auto* samples = bank.getWavetable(waveID).getReadPointer(0);
    for (int i = 0; i < numSamples; ++i) {
        waveform.add(samples[i]);
    }
    
    juce::Path path;
    path.startNewSubPath(0, height / 2);

    for (int i = 0; i < waveform.size(); ++i) {
        float x = juce::jmap((float)i, 0.0f, (float)waveform.size(), 0.0f, (float)width);
        float y = juce::jmap(waveform[i], -1.0f, 1.0f, (float)height, 0.0f);
        path.lineTo(x, y);
    }

    g.setColour(juce::Colours::aquamarine);
    g.strokePath(path, juce::PathStrokeType(2.0f));
}

void WaveDiagram::resized() {

}