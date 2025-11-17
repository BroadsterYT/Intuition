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
    const auto shadow = juce::Colour::fromRGB(28, 24, 22);
    const auto charcoal = juce::Colour::fromRGB(32, 26, 22);

    /// <summary>
    /// Draws a waveform that looks like light casted from a lightbulb in a dark room
    /// </summary>
    /// <param name="g">The JUCE graphics context</param>
    /// <param name="waveformPath">The waveform path to draw</param>
    /// <param name="bounds">The bounds of the component the waveform is being drawn within</param>
    /// <param name="radial">If true, will draw the light source at the center of the component instead of the top</param>
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

    /// <summary>
    /// Draws a waveform shape that looks like a lightbulb filament
    /// </summary>
    /// <param name="g">The JUCE graphics context</param>
    /// <param name="waveformPath">The path to draw</param>
    /// <param name="bounds">The bounds of the component the waveform is being drawn within</param>
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

    /// <summary>
    /// Draws an indicator bar, like an LFO phase or an envelope position
    /// </summary>
    /// <param name="g">The JUCE graphics context</param>
    /// <param name="indicatorPath">The path of the indicator (the line to draw)</param>
    /// <param name="xPos">The x-axis psoition to draw the indicator</param>
    inline void drawRadiantIndicator(
        juce::Graphics& g,
        juce::Path& indicatorPath,
        float xPos) {

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

    inline void drawRadiantPoint(
        juce::Graphics& g,
        float posX, float posY,
        float radius, float brightness) {
        juce::ColourGradient gradient(
            GlowStyle::bulbGlow.withAlpha(1.0f),
            posX,
            posY,
            GlowStyle::roomDark.withAlpha(0.2f),
            posX + radius,
            posY,
            true
        );
        gradient.addColour(0.4f, GlowStyle::warmHighlight.withAlpha(0.3f + 0.7f * brightness));
        gradient.addColour(1.0f, GlowStyle::roomDark);

        juce::Rectangle<float> bounds = { posX - radius / 2.0f, posY - radius / 2.0f, radius, radius };
        g.setGradientFill(gradient);
        g.fillEllipse(bounds);
    }

    inline void drawRadiantRing(
        juce::Graphics& g,
        float posX, float posY,
        float radius, float brightness) {
        
        g.setColour(GlowStyle::warmHighlight.withAlpha(brightness));
        g.drawEllipse(posX, posY, radius * 2.0f, radius * 2.0f, 2.0f);
    }
}