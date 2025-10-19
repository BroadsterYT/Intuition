/*
  ==============================================================================

    FilterGraph.cpp
    Created: 19 Oct 2025 3:37:10pm
    Author:  BroDe

  ==============================================================================
*/

#include "FilterGraph.h"

FilterGraph::FilterGraph(
    juce::AudioProcessorValueTreeState& vts
) : parameters(vts) {
    startTimerHz(30);
}

void FilterGraph::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);
    
    float cutoff = *parameters.getRawParameterValue("FILTER_CUTOFF");
    float resonance = *parameters.getRawParameterValue("FILTER_RESONANCE");
    int typeChoice = *parameters.getRawParameterValue("FILTER_TYPE");
    
    g.setColour(juce::Colours::lime);
    drawFrequencyResponse(g, cutoff, resonance, typeChoice);
}

void FilterGraph::timerCallback() {
    repaint();
}

float FilterGraph::magnitudeToDecibels(float mag) {
    return 20.0f * std::log10(mag + 1e-6f);
}

float FilterGraph::getLowpassMagnitude(float freq, float cutoff, float resonance) {
    float mag = 1.0f / std::sqrt(1.0f + std::pow(freq / cutoff, 2.0f) * (1.0f + resonance));
    return magnitudeToDecibels(mag);
}

float FilterGraph::getHighpassMagnitude(float freq, float cutoff, float resonance) {
    float mag = 1.0f / std::sqrt(1.0f + std::pow(cutoff / freq, 2.0f) * (1.0f + resonance));
    return magnitudeToDecibels(mag);
}

float FilterGraph::getBandpassMagnitude(float freq, float cutoff, float resonance) {
    float Q = 1.0f + resonance * 19.0f;
    float ratio = freq / cutoff;
    float mag = 1.0f / std::sqrt(1.0f + Q * Q * (ratio - 1.0f / ratio) * (ratio - 1.0f / ratio));
    return magnitudeToDecibels(mag);
}

void FilterGraph::drawFrequencyResponse(juce::Graphics& g, float cutoff, float resonance, int filterType) {
    int width = getWidth();
    int height = getHeight();
    juce::Path path;

    for (int i = 0; i < width; ++i) {
        float freq = 20.0f * std::pow(20000.0f / 20.0f, (float)i / (float)width);
        float mag = 1.0f;

        switch (filterType) {
        case 0:  // Lowpass
            mag = getLowpassMagnitude(freq, cutoff, resonance);
            break;
        case 1:  // Highpass
            mag = getHighpassMagnitude(freq, cutoff, resonance);
            break;
        case 2:  // Bandpass
            mag = getBandpassMagnitude(freq, cutoff, resonance);
            break;
        }

        float minDb = -24.0f;
        float maxDb = 24.0f;
        float y = (1.0f - (mag - minDb) / (maxDb - minDb)) * height;

        if (i == 0) {
            path.startNewSubPath((float)i, y);
        }
        else {
            path.lineTo((float)i, y);
        }
    }

    g.strokePath(path, juce::PathStrokeType(2.0f));
}
