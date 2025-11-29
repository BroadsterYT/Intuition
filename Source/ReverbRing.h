/*
  ==============================================================================

    ReverbRing.h
    Created: 15 Nov 2025 4:01:58pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ModMatrix.h"
#include "ItnLookAndFeel.h"


class ReverbRing : public juce::Component, private juce::Timer {
public:
    ReverbRing(ModMatrix* modMatrix);
    ~ReverbRing();

    float getRadius();

    void setBrightness(float newBrightness);
    float getBrightness();

    void paint(juce::Graphics& g) override;

private:
    ModMatrix* modMatrix = nullptr;

    float radius = 10.0f;
    float growthRate = 1.0f;
    float brightness = 1.0f;

    void timerCallback() override;
};