/*
  ==============================================================================

    ReverbRing.cpp
    Created: 15 Nov 2025 4:05:15pm
    Author:  BroDe

  ==============================================================================
*/

#include "ReverbRing.h"

ReverbRing::ReverbRing(ModMatrix* modMatrix) : modMatrix(modMatrix) {
    setLookAndFeel(&lookAndFeel);
    startTimerHz(60);
}

ReverbRing::~ReverbRing() {
    setLookAndFeel(nullptr);
}

float ReverbRing::getRadius() {
    return radius;
}

void ReverbRing::setBrightness(float newBrightness) {
    brightness = newBrightness;
}

float ReverbRing::getBrightness() {
    return brightness;
}

void ReverbRing::paint(juce::Graphics& g) {
    float w = getWidth();
    float h = getHeight();
    GlowStyle::drawRadiantRing(g, w / 2 - radius, h / 2 - radius, radius, brightness);
}

void ReverbRing::timerCallback() {
    float roomSize = modMatrix->getModdedDest("REVERB_ROOM_SIZE");
    growthRate = juce::jmap(roomSize, 0.5f, 2.5f);
    radius += growthRate;
}
