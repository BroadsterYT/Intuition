/*
  ==============================================================================

    LFODisplay.cpp
    Created: 18 Oct 2025 7:25:19pm
    Author:  BroDe

  ==============================================================================
*/

#include "LFODisplay.h"

LFODisplay::LFODisplay(
    juce::AudioProcessorValueTreeState& vts,
    LFOShape& shape, float* phase,
    const juce::String modeName,
    const juce::String rateName,
    const juce::String syncDivName
) : parameters(vts),
    editor(shape, phase), 
    modeName(modeName),
    syncDiv(vts, syncDivName),
    
    rate(vts, rateName, "LFO RATE", "LFO_RATE", "Hz") {

    mode.addItem("Free Run", 1);
    mode.addItem("BPM Sync", 2);
    mode.setSelectedId(1);
    mode.onChange = [this] { modeChanged(); };

    modeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(parameters, modeName, mode);
    //syncDivAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(parameters, syncDivName, syncDiv);

    rate.setRange(0.01f, 30.0f);

    addAndMakeVisible(editor);
    addAndMakeVisible(mode);
    addAndMakeVisible(syncDiv);
    addChildComponent(rate);
}

void LFODisplay::paint(juce::Graphics& g) {
    MinimalStyle::drawCustomRoundedPanel(g, getLocalBounds().toFloat(), MinimalStyle::bgPanel, true, true, false, false);
}

void LFODisplay::resized() {
    auto area = getLocalBounds();
    auto optionsArea = area.removeFromBottom(100).reduced(10);
    
    int split = optionsArea.getWidth() / 3;
    mode.setBounds(optionsArea.removeFromLeft(split));
    syncDiv.setBounds(optionsArea.removeFromLeft(split));
    rate.setBounds(optionsArea.removeFromLeft(split));

    editor.setBounds(area.reduced(10));
}

void LFODisplay::modeChanged() {
    int id = mode.getSelectedId();
    if (id == 1) {
        rate.setVisible(true);
        syncDiv.setVisible(false);
    }
    else if (id == 2) {
        rate.setVisible(false);
        syncDiv.setVisible(true);
    }
}
