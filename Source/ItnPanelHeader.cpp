/*
  ==============================================================================

    ItnPanelHeader.cpp
    Created: 3 Dec 2025 1:52:59pm
    Author:  BroDe

  ==============================================================================
*/

#include "ItnPanelHeader.h"


ItnPanelHeader::ItnPanelHeader(juce::AudioProcessorValueTreeState& vts, const juce::String title, bool includeToggle, const juce::String toggleParamName) : parameters(vts), title(title), toggleParamName(toggleParamName) {
    if (includeToggle) {
        addAndMakeVisible(toggle);

    }
}

void ItnPanelHeader::paint(juce::Graphics& g) {

}

void ItnPanelHeader::resized() {

}
