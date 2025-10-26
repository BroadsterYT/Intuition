/*
  ==============================================================================

    ItnLookAndFeelHelpers.h
    Created: 25 Oct 2025 11:22:37pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "LFOShape.h"


namespace GlowStyle {
    const auto roomDark = juce::Colour::fromRGB(18, 14, 10);
    const auto warmHighlight = juce::Colour::fromRGB(255, 204, 102);
    const auto bulbGlow = juce::Colour::fromRGB(255, 248, 225);
    const auto shadow = juce::Colour::fromRGB(43, 29, 20);

    inline void drawRadiantWaveform(
        juce::Graphics& g,
        juce::Path& waveformPath,
        juce::Rectangle<float> bounds,
        bool radial = false) {
        juce::ColourGradient gradient;
        if (!radial) {
            gradient = juce::ColourGradient::vertical(
                GlowStyle::bulbGlow.withAlpha(0.8f),
                bounds.getY(),
                GlowStyle::roomDark.withAlpha(0.2f),
                bounds.getBottom()
            );
        }
        else {
            auto pathBounds = waveformPath.getBounds();

            float centerX = pathBounds.getCentreX();
            float centerY = pathBounds.getCentreY();
            float radius = bounds.getWidth() * 0.75f;

            gradient = juce::ColourGradient(
                GlowStyle::bulbGlow.withAlpha(0.8f),
                centerX,
                centerY,
                GlowStyle::roomDark.withAlpha(0.2f),
                centerX + radius,
                centerY,
                true
            );
        }
        gradient.addColour(0.2f, GlowStyle::warmHighlight.withAlpha(0.7f));

        g.setGradientFill(gradient);
        g.fillPath(waveformPath);
    }

    inline void drawFilamentWaveform(
        juce::Graphics& g,
        juce::Path& waveformPath,
        juce::Rectangle<float> bounds) {
        juce::ColourGradient gradient = juce::ColourGradient::horizontal(
            GlowStyle::bulbGlow.withAlpha(0.8f),
            bounds.getX(),
            GlowStyle::bulbGlow.withAlpha(0.2f),
            bounds.getRight()
        );
        gradient.addColour(0.5f, GlowStyle::shadow);

        g.setGradientFill(gradient);
        g.strokePath(waveformPath, juce::PathStrokeType(2.0f));
    }

    inline void drawRadiantIndicator(
        juce::Graphics& g,
        juce::Path& indicatorPath,
        float xPos, float height) {

        juce::ColourGradient gradient = juce::ColourGradient::horizontal(
            GlowStyle::bulbGlow.withAlpha(0.0f),
            xPos - 6.0f,
            GlowStyle::bulbGlow.withAlpha(0.0f),
            xPos + 6.0f
        );
        gradient.addColour(0.35f, GlowStyle::bulbGlow.withAlpha(0.25f));
        gradient.addColour(0.5f, GlowStyle::bulbGlow);
        gradient.addColour(0.65f, GlowStyle::bulbGlow.withAlpha(0.25f));
        g.setGradientFill(gradient);
        g.strokePath(indicatorPath, juce::PathStrokeType(12.0f));
    }
}