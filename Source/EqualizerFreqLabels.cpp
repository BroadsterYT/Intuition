/*
  ==============================================================================

    EqualizerFreqLabels.cpp
    Created: 29 Mar 2026 4:35:57pm
    Author:  BroDe

  ==============================================================================
*/

#include "EqualizerFreqLabels.h"
#include "BiquadResponse.h"

EqualizerFreqLabels::EqualizerFreqLabels() {
    setLookAndFeel(&ItnLookAndFeel::getInstance());
}

EqualizerFreqLabels::~EqualizerFreqLabels() {
    setLookAndFeel(nullptr);
}

void EqualizerFreqLabels::paint(juce::Graphics& g) {
    g.fillAll(MinimalStyle::bgDarkest);

    // Drawing C note references
    /*const float layer1Y = 0.0f;
    for (int i = 0; i <= 10; ++i) {
        float minFreq = BiquadResponse::getNthOctaveFreq(i, 9);
        float maxFreq = BiquadResponse::getNthOctaveFreq(i + 1, 9);
        bool noSpace = false;
        juce::String text;
        text << "  C" << i;
        if (i == 0) {
            text = "";
            minFreq = 20.0f;
        }
        else if (i == 10) {
            noSpace = true;
            maxFreq = 20000.0f;
        }
        drawRangeBox(g, layer1Y, minFreq, maxFreq, text, noSpace, 2.0f, juce::Justification::centredLeft);
    }*/

    // Drawing frequency ranges
    const float layer2Y = 14.0f;
    drawRangeBox(g, layer2Y, 20.0f, 60.0f, "Sub-Bass");  // 20Hz - 60Hz
    drawRangeBox(g, layer2Y, 60.0f, 250.0f, "Bass");  // 60Hz - 250Hz
    drawRangeBox(g, layer2Y, 250.0f, 500.0f, "Low-Mids");  // 250Hz - 500Hz
    drawRangeBox(g, layer2Y, 500.0f, 2000.0f, "Center-Mids");  // 500Hz - 2kHz
    drawRangeBox(g, layer2Y, 2000.0f, 4000.0f, "High-Mids");  // 2kHz - 4kHz
    drawRangeBox(g, layer2Y, 4000.0f, 6000.0f, "Presence");  // 4kHz - 6kHz
    drawRangeBox(g, layer2Y, 6000.0f, 20000.0f, "Brilliance", true);  // 6kHz - 20kHz

    // Drawing actual frequencies
    const float layer3Y = 0.0f;
    drawRangeBox(g, layer3Y, 20.0f, 50.0f, "  20Hz", false, 2.0f, juce::Justification::centredLeft);
    drawRangeBox(g, layer3Y, 50.0f, 100.0f, "  50Hz", false, 2.0f, juce::Justification::centredLeft);
    drawRangeBox(g, layer3Y, 100.0f, 200.0f, "  100Hz", false, 2.0f, juce::Justification::centredLeft);
    drawRangeBox(g, layer3Y, 200.0f, 500.0f, "  200Hz", false, 2.0f, juce::Justification::centredLeft);
    drawRangeBox(g, layer3Y, 500.0f, 1000.0f, "  500Hz", false, 2.0f, juce::Justification::centredLeft);
    drawRangeBox(g, layer3Y, 1000.0f, 2000.0f, "  1kHz", false, 2.0f, juce::Justification::centredLeft);
    drawRangeBox(g, layer3Y, 2000.0f, 5000.0f, "  2kHz", false, 2.0f, juce::Justification::centredLeft);
    drawRangeBox(g, layer3Y, 5000.0f, 10000.0f, "  5kHz", false, 2.0f, juce::Justification::centredLeft);
    drawRangeBox(g, layer3Y, 10000.0f, 20000.0f, "  10kHz", true, 2.0f, juce::Justification::centredLeft);
}

void EqualizerFreqLabels::drawRangeBox(juce::Graphics& g, const float posY, float freqMin, float freqMax, const juce::String& text, const bool noSpace, const float xSpace, juce::Justification just) {
    float width = (float)getWidth();
    float posMin = BiquadResponse::getFreqInLinearRange(freqMin, width);
    float posMax = BiquadResponse::getFreqInLinearRange(freqMax, width);

    // Leaving room for gap between next range
    float xLimit;
    if (noSpace) {
        xLimit = posMax;
    }
    else {
        xLimit = BiquadResponse::getLinearValueAsFreq(posMax - xSpace, width);
        xLimit = BiquadResponse::getFreqInLinearRange(xLimit, width);
    }

    auto rect = juce::Rectangle<float>(posMin, posY, xLimit - posMin, 12.0f);
    g.setFont(ItnLookAndFeel::getInstance().getGraphFont());
    g.setColour(MinimalStyle::textSecondary);
    g.drawRect(rect, 1.0f);
    g.drawText(text, rect, just);
}