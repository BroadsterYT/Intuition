/*
  ==============================================================================

    DelayRing.h
    Created: 16 Nov 2025 4:00:07pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ModMatrix.h"
#include "ItnLookAndFeel.h"


class DelayRing : public juce::Component, private juce::Timer {
public:
    DelayRing(ModMatrix* modMatrix);
    ~DelayRing();

    float getRadius();

    void setBrightness(float newBrightness);
    float getBrightness();

    void paint(juce::Graphics& g) override;

private:
    ItnLookAndFeel lookAndFeel;
    ModMatrix* modMatrix = nullptr;

    float radius = 10.0f;
    float growthRate = 1.0f;
    float brightness = 1.0f;

    void timerCallback() override;
};