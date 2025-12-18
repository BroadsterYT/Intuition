/*
  ==============================================================================

    DelayRing.cpp
    Created: 16 Nov 2025 4:00:07pm
    Author:  BroDe

  ==============================================================================
*/

#include "DelayRing.h"

DelayRing::DelayRing(ModMatrix* modMatrix) : modMatrix(modMatrix) {
    setLookAndFeel(&ItnLookAndFeel::getInstance());
    startTimerHz(60);
}

DelayRing::~DelayRing() {
    setLookAndFeel(nullptr);
}

float DelayRing::getRadius() {
    return radius;
}

void DelayRing::setBrightness(float newBrightness) {
    brightness = newBrightness;
}

float DelayRing::getBrightness() {
    return brightness;
}

void DelayRing::paint(juce::Graphics& g) {
    float w = getWidth();
    float h = getHeight();

    float realRad = radius + 50.0f;
    MinimalStyle::drawRadiantRing(g, 0.0f - realRad - 50.0f, h / 2 - realRad, realRad, brightness);
}

void DelayRing::timerCallback() {
    float feedback = modMatrix->getModdedDest("DELAY_FEEDBACK");
    growthRate = juce::jmap(feedback, 0.5f, 2.5f);
    radius += growthRate;
}
