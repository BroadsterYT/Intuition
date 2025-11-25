/*
  ==============================================================================

    WaveBankComponent.cpp
    Created: 3 Oct 2025 9:11:09pm
    Author:  Brody

  ==============================================================================
*/

#include "WaveBankComponent.h"


WaveBankComponent::WaveBankComponent(
    juce::AudioProcessor* ap,
    juce::AudioProcessorValueTreeState& vts,
    WavetableBank& bank
)   : parameters(vts),
    bank(bank) {

    processor = dynamic_cast<IntuitionAudioProcessor*>(ap);
    
    addAndMakeVisible(viewport);
    viewport.setViewedComponent(&waveThumbnailComp, false);

    addAndMakeVisible(closeButton);
    closeButton.setButtonText("X");
    closeButton.onClick = [this] {
        setVisible(false);
    };

    waveThumbnailComp.addAndMakeVisible(addNewWaveButton);
    addNewWaveButton.setButtonText("Add New");
    addNewWaveButton.onClick = [this, &bank] () {
        juce::File home = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("Intuition");
        juce::File waveforms = home.getChildFile("Waveforms");
        
        auto chooser = std::make_shared<juce::FileChooser>(
            "Select a WAV file to open...",
            waveforms,
            "*.wav"
        );

        chooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectMultipleItems,
            [this, chooser, &bank] (const juce::FileChooser& fc) {
                auto files = fc.getResults();
                if (files.size() <= 0) {
                    DBG("User cancelled file selection.");
                    return;
                }

                for (auto& file : files) {
                    bank.addWavetable(file);
                }
                processor->resetSynths();
                buildWaveThumbnails();
            }
        );
    };
}

void WaveBankComponent::resetProcessorSynths() {
    processor->resetSynths();
}

void WaveBankComponent::buildWaveThumbnails() {
    waveThumbnails.clear(true);

    int i = 0;
    for (i = 0; i < bank.size(); ++i) {
        auto* newWave = new WaveThumbnail(i, bank);
        newWave->setBounds(15 * (i + 1) + 80 * i, 15, 80, 80);
        waveThumbnailComp.addAndMakeVisible(newWave);
        waveThumbnails.add(newWave);
    }

    addNewWaveButton.setBounds(15 * (i + 1) + 80 * i, waveThumbnailComp.getHeight() / 2 - 24, 80, 32);
}

void WaveBankComponent::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::darkgrey);
    g.fillRect(getLocalBounds());
}

void WaveBankComponent::resized() {
    auto area = getLocalBounds();
    area.removeFromBottom(90); // Keyboard

    viewport.setBounds(area.removeFromBottom(128).reduced(10));
    waveThumbnailComp.setBounds(0, 0, 1600, 80 + 15);
    closeButton.setBounds(getWidth() - 32 - 16, 64, 32, 32);

    buildWaveThumbnails();
}