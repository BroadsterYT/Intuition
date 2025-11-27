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
    LFOShape& shape,
    float* phase,
    const juce::String modeName,
    const juce::String rateName,
    const juce::String syncDivName
) : parameters(vts),
    editor(shape, phase), 
    modeName(modeName),
    rateName(rateName),
    syncDivName(syncDivName) {

    mode.addItem("Free Run", 1);
    mode.addItem("BPM Sync", 2);
    mode.setSelectedId(1);
    mode.onChange = [this] { modeChanged(); };

    syncDiv.addItem("4 bars", 1);
    syncDiv.addItem("2 bars", 2);
    syncDiv.addItem("1 bar", 3);
    syncDiv.addItem("1/2", 4);
    syncDiv.addItem("1/4", 5);
    syncDiv.addItem("1/8", 6);
    syncDiv.addItem("1/16", 7);
    syncDiv.addItem("1/32", 8);

    modeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(parameters, modeName, mode);
    syncDivAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(parameters, syncDivName, syncDiv);
    rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(parameters, rateName, rate);

    rate.setRange(0.01f, 30.0f);

    addAndMakeVisible(editor);
    addAndMakeVisible(mode);
    addAndMakeVisible(syncDiv);
    addChildComponent(rate);

    rate.setTooltipFields(
        "LFO Rate",
        "Certified wobble.",
        "The LFO rate is how many cycles per second the LFO completes. This dial controls the \"unsynced\" LFO rate, meaning that the rate may not align with the song's BPM."
    );
}

void LFODisplay::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::darkgrey);
    g.fillRect(getLocalBounds());
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
