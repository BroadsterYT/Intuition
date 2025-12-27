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


namespace MinimalStyle {
    const auto roomDark = juce::Colour::fromRGB(18, 14, 10);
    const auto warmHighlight = juce::Colour::fromRGB(255, 204, 102);
    const auto bulbGlow = juce::Colour::fromRGB(255, 248, 225);
    const auto shadow = juce::Colour::fromRGB(28, 24, 22);
    const auto charcoal = juce::Colour::fromRGB(32, 26, 22);
    const auto gray = juce::Colour::fromRGB(176, 167, 157);

    // Background Colors
    const auto bgDarkest = juce::Colour::fromRGB(13, 13, 15);
    const auto bgDark = juce::Colour::fromRGB(20, 20, 24);
    const auto bgPanel = juce::Colour::fromRGB(26, 26, 31);
    const auto bgModule = juce::Colour::fromRGB(34, 34, 40);
    const auto bgControl = juce::Colour::fromRGB(42, 42, 50);

    // Border Colors
    const auto borderDark = juce::Colour::fromRGB(45, 45, 53);
    const auto borderLight = juce::Colour::fromRGB(61, 61, 69);

    // Text Colors
    const auto textPrimary = juce::Colour::fromRGB(245, 245, 247);
    const auto textSecondary = juce::Colour::fromRGB(160, 160, 168);
    const auto textDim = juce::Colour::fromRGB(106, 106, 114);

    // Accent Colors
    const auto accentPeach = juce::Colour::fromRGB(255, 176, 136);
    const auto accentOrange = juce::Colour::fromRGB(255, 140, 90);
    const auto accentWarm = juce::Colour::fromRGB(255, 160, 112);
    const auto accentGlow = juce::Colour::fromRGBA(255, 140, 90, 77);
    const auto accentSubtle = juce::Colour::fromRGBA(255, 176, 136, 26);

    /// <summary>
    /// Draws a rounded rectangle for a component panel with select corners rounded and sharp
    /// </summary>
    /// <param name="g">The JUCE graphics context to use</param>
    /// <param name="bounds">The bounds to draw within</param>
    /// <param name="insideColor">The color to fill the rectangle with</param>
    /// <param name="sharpTopLeft">Make true if the top left corner should be a right angle. False to round it.</param>
    /// <param name="sharpTopRight">Make true if the top right corner should be a right angle. False to round it.</param>
    /// <param name="sharpBottomLeft">Make true if the bottom left corner should be a right angle. False to round it.</param>
    /// <param name="sharpBottomRight">Make true if the bottom right corner should be a right angle. False to round it.</param>
    inline void drawCustomRoundedPanel(juce::Graphics& g, juce::Rectangle<float> bounds, juce::Colour insideColor, bool sharpTopLeft, bool sharpTopRight, bool sharpBottomLeft, bool sharpBottomRight) {
        g.setColour(borderDark);
        g.fillRoundedRectangle(bounds, 10.0f);
        g.setColour(insideColor);
        g.fillRoundedRectangle(bounds.reduced(1.0f), 10.0f);

        float width = bounds.getWidth();
        float height = bounds.getHeight();
        if (sharpTopLeft) {
            g.setColour(borderDark);
            g.fillRect(0.0f, 0.0f, 10.0f, 10.0f);
            g.setColour(insideColor);
            g.fillRect(1.0f, 1.0f, 10.0f, 10.0f);
        }
        if (sharpTopRight) {
            g.setColour(borderDark);
            g.fillRect(width - 10.0f, 0.0f, 10.0f, 10.0f);
            g.setColour(insideColor);
            g.fillRect(width - 10.0f, 1.0f, 9.0f, 9.0f);
        }
        if (sharpBottomLeft) {
            g.setColour(borderDark);
            g.fillRect(0.0f, height - 10.0f, 10.0f, 10.0f);
            g.setColour(insideColor);
            g.fillRect(1.0f, height - 11.0f, 10.0f, 10.0f);
        }
        if (sharpBottomRight) {
            g.setColour(borderDark);
            g.fillRect(width - 10.0f, height - 10.0f, 10.0f, 10.0f);
            g.setColour(insideColor);
            g.fillRect(width - 10.0f, height - 10.0f, 9.0f, 9.0f);
        }
    }

    /// <summary>
    /// Draws a single-line waveform shape
    /// </summary>
    /// <param name="g">The JUCE graphics context</param>
    /// <param name="waveformPath">The path to draw</param>
    inline void drawLineWaveform(juce::Graphics& g, juce::Path& waveformPath) {
        g.setColour(MinimalStyle::accentPeach);
        g.strokePath(waveformPath, juce::PathStrokeType(2.0f));
    }

    /// <summary>
    /// Draws a waveform with a gradient fill
    /// </summary>
    /// <param name="g">The JUCE graphics context</param>
    /// <param name="waveformPath">The waveform path to draw</param>
    /// <param name="bounds">The bounds of the component the waveform is being drawn within</param>
    /// <param name="radial">If true, will draw the light source at the center of the component instead of the top</param>
    inline void drawGradientWaveform(juce::Graphics& g, juce::Path& waveformPath, juce::Rectangle<float> bounds) {
        juce::ColourGradient gradient = juce::ColourGradient::vertical(
            MinimalStyle::accentGlow,
            bounds.getY(),
            MinimalStyle::accentSubtle,
            bounds.getBottom()
        );
        g.setGradientFill(gradient);
        g.fillPath(waveformPath);
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
            MinimalStyle::bulbGlow.withAlpha(0.0f),
            xPos - 6.0f,
            MinimalStyle::bulbGlow.withAlpha(0.0f),
            xPos + 6.0f
        );
        gradient.addColour(0.35f, MinimalStyle::bulbGlow.withAlpha(0.25f));
        gradient.addColour(0.5f, MinimalStyle::bulbGlow);
        gradient.addColour(0.65f, MinimalStyle::bulbGlow.withAlpha(0.25f));
        g.setGradientFill(gradient);
        g.strokePath(indicatorPath, juce::PathStrokeType(12.0f));
    }

    inline void drawRadiantPoint(juce::Graphics& g, float posX, float posY, float radius) {
        juce::Rectangle<float> bounds = { posX - radius / 2.0f, posY - radius / 2.0f, radius, radius };
        g.setColour(accentOrange);
        g.fillEllipse(bounds);
    }

    inline void drawRadiantRing(
        juce::Graphics& g, float posX, float posY, float radius, float brightness) {
        g.setColour(MinimalStyle::warmHighlight.withAlpha(brightness));
        g.drawEllipse(posX, posY, radius * 2.0f, radius * 2.0f, 2.0f);
    }
}