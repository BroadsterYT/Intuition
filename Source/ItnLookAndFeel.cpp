/*
  ==============================================================================

    ItnLookAndFeel.cpp
    Created: 29 Sep 2025 1:08:01am
    Author:  BroDe

  ==============================================================================
*/

#include "ItnLookAndFeel.h"


ItnLookAndFeel::ItnLookAndFeel() {
    exo2TypeFaceRegular = juce::Typeface::createSystemTypefaceFor(BinaryData::Exo2Regular_ttf, BinaryData::Exo2Regular_ttfSize);
    exo2TypeFaceBold = juce::Typeface::createSystemTypefaceFor(BinaryData::Exo2Bold_ttf, BinaryData::Exo2Bold_ttfSize);
    exo2TypeFaceItalic = juce::Typeface::createSystemTypefaceFor(BinaryData::Exo2Italic_ttf, BinaryData::Exo2Italic_ttfSize);
    outfitTypeFaceRegular = juce::Typeface::createSystemTypefaceFor(BinaryData::OutfitRegular_ttf, BinaryData::OutfitRegular_ttfSize);
    outfitTypeFaceBold = juce::Typeface::createSystemTypefaceFor(BinaryData::OutfitBold_ttf, BinaryData::OutfitBold_ttfSize);
    jetBrainsMonoTypeFaceRegular = juce::Typeface::createSystemTypefaceFor(BinaryData::JetBrainsMonoRegular_ttf, BinaryData::JetBrainsMonoRegular_ttfSize);
}

ItnLookAndFeel& ItnLookAndFeel::getInstance() {
    static ItnLookAndFeel instance;
    return instance;
}

void ItnLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) {
    auto radius = juce::jmin(width / 2.0f, height / 2.0f) - 8.0f;
    auto centerX = x + width * 0.5f;
    auto centerY = y + height * 0.5f;

    juce::Path baseArc;
    baseArc.addCentredArc(centerX, centerY, radius, radius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
    g.setColour(GlowStyle::shadow);
    g.strokePath(baseArc, juce::PathStrokeType(8.0f));

    float fillAngle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    juce::Path filledArc;
    filledArc.addCentredArc(centerX, centerY, radius, radius, 0.0f, rotaryStartAngle, fillAngle, true);
    
    juce::ColourGradient gradient = juce::ColourGradient::horizontal(
        GlowStyle::shadow,
        (float)x,
        GlowStyle::warmHighlight,
        (float)(x + width)
    );
    gradient.addColour(0.35f, GlowStyle::shadow);
    g.setGradientFill(gradient);
    g.strokePath(filledArc, juce::PathStrokeType(8.0f));

    float angle = 3.0f * juce::MathConstants<float>::pi / 2.0f + rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    float knobRadius = 6.0f;
    float knobX = centerX + std::cos(angle) * radius;
    float knobY = centerY + std::sin(angle) * radius;
    GlowStyle::drawRadiantPoint(g, knobX, knobY, knobRadius * 2.0f, sliderPosProportional);
}

juce::Font ItnLookAndFeel::getLabelFont(juce::Label& label) {
    auto id = label.getComponentID();
    if (id == "PanelTitle") {
        return juce::Font(outfitTypeFaceBold).withHeight(18.0f);
    }
    return juce::Font(outfitTypeFaceRegular).withHeight(14.0f);
}

juce::Font ItnLookAndFeel::getTooltipHeaderFont(float height) {
    return juce::Font(exo2TypeFaceBold).withHeight(height);
}

juce::Font ItnLookAndFeel::getTooltipSubheaderFont(float height) {
    return juce::Font(exo2TypeFaceItalic).withHeight(height);
}

juce::Font ItnLookAndFeel::getTooltipDescriptionFont(float height) {
    return juce::Font(exo2TypeFaceRegular).withHeight(height);
}

void ItnLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label) {
    const auto id = label.getComponentID();

    if (id == "sliderNameLabel") {
        juce::LookAndFeel_V4::drawLabel(g, label);
    }
    else if (id == "sliderValueBox") {
        drawSliderValueBox(g, label);  // TODO: Replace with custom draw method
    }
    else {
        juce::LookAndFeel_V4::drawLabel(g, label);
    }
}

void ItnLookAndFeel::drawComponentPanel(juce::Graphics& g, juce::Rectangle<float> bounds, const juce::Colour insideColor, bool includeLabelArea, float labelHeight) {
    g.setColour(GlowStyle::borderDark);
    g.fillRoundedRectangle(bounds, 10.0f);
    g.setColour(insideColor);
    g.fillRoundedRectangle(bounds.reduced(1.0f), 10.0f);

    if (includeLabelArea) {
        g.setColour(GlowStyle::borderDark);
        g.fillRect(0.0f, labelHeight, bounds.getRight(), 1.0f);
    }
}

void ItnLookAndFeel::drawWaveform(juce::Graphics& g,juce::Rectangle<float> bounds, const juce::Array<float> waveform) {
    int width = bounds.getWidth();
    int height = bounds.getHeight();
    
    juce::Path fill, line;
    fill.startNewSubPath(0, height / 2);
    line.startNewSubPath(0, height / 2);

    for (int i = 0; i < waveform.size(); ++i) {
        float x = juce::jmap((float)i, 0.0f, (float)waveform.size(), 0.0f, (float)width);
        float y = juce::jmap(waveform[i], -1.0f, 1.0f, (float)height - 5.0f, 5.0f);
        fill.lineTo(x, y);
        line.lineTo(x, y);
    }
    fill.lineTo(width, height / 2);
    line.lineTo(width, height / 2);

    //fill.closeSubPath();
    //GlowStyle::drawRadiantWaveform(g, fill, bounds, true);
    GlowStyle::drawLineWaveform(g, line);
}

void ItnLookAndFeel::drawEnvelope(juce::Graphics& g, juce::Rectangle<float> bounds, float attack, float decay, float sustain, float release, float envTime) {
    int width = bounds.getWidth();
    int height = bounds.getHeight();

    float segWidth = (float)width / 3;  // TODO: Replace 3 with time multiplier

    float attackX = attack * segWidth;
    float decayX = decay * segWidth;
    float sustainY = height * (1.0f - sustain);
    float releaseX = release * segWidth;

    float decayEndX = attackX + decayX;
    float releaseEndX = decayEndX + releaseX;

    juce::Path envLine;
    envLine.startNewSubPath(0.0f, height);
    envLine.lineTo(attackX, 0.0f);
    envLine.lineTo(decayEndX, sustainY);
    envLine.lineTo(releaseEndX, height);

    GlowStyle::drawRadiantWaveform(g, envLine, bounds);

    if (envTime <= 0.0f) return;
    float timeSeg = envTime * segWidth;

    float finalX = timeSeg * (attack + decay + release);
    juce::Path timePath;
    timePath.startNewSubPath(finalX, 0.0f);
    timePath.lineTo(finalX, bounds.getBottom());
    GlowStyle::drawRadiantIndicator(g, timePath, finalX);
}

void ItnLookAndFeel::drawLFO(juce::Graphics& g, juce::Rectangle<float> bounds, LFOShape& shape, float phase) {
    float width = bounds.getWidth();
    float height = bounds.getHeight();
    
    juce::Path path;
    auto& p0 = shape.getPoint(0);
    float x0 = p0.getTime() * width;
    float y0 = (1.0f - p0.getValue()) * height;
    path.startNewSubPath(x0, y0);

    for (int i = 1; i < shape.getNumPoints(); ++i) {
        auto& p1 = shape.getPoint(i - 1);
        auto& p2 = shape.getPoint(i);

        x0 = p1.getTime() * width;
        y0 = (1.0f - p1.getValue()) * height;

        float x2 = p2.getTime() * width;
        float y2 = (1.0f - p2.getValue()) * height;

        float cx = x0 + (x2 - x0) / 2.0f;
        float cy = y0 + (y2 - y0) / 2.0f - p1.getCurve() * height;

        path.quadraticTo(cx, cy, x2, y2);
    }
    path.lineTo(width, height);
    path.lineTo(0.0f, height);

    GlowStyle::drawRadiantWaveform(g, path, bounds);

    juce::Path phaseInd;
    float xPos = phase * bounds.getWidth();
    phaseInd.startNewSubPath(xPos, 0.0f);
    phaseInd.lineTo(xPos, bounds.getHeight());
    GlowStyle::drawRadiantIndicator(g, phaseInd, xPos);
}

void ItnLookAndFeel::drawLFOPoint(juce::Graphics& g, juce::Rectangle<float> bounds, const LFOPoint& point) {
    float posX = point.getTime() * bounds.getWidth();
    float posY = point.getValue() * bounds.getHeight();

    float brightness = (posY) / bounds.getHeight();
    GlowStyle::drawRadiantPoint(g, posX, bounds.getHeight() - posY, 16.0f, brightness);
}

void ItnLookAndFeel::drawFilter(juce::Graphics& g, juce::Rectangle<float> bounds, float cutoff, float resonance, int filterType) {
    float width = bounds.getWidth();
    float height = bounds.getHeight();
    juce::Path path;

    auto magToDecibels = [&](float magnitude) {
        return 20.0f * std::log10(magnitude + 1e-6f);
    };

    for (int i = 0; i < width; ++i) {
        float freq = 20.0f * std::pow(20000.0f / 20.0f, (float)i / (float)width);
        float mag = 1.0f;

        // Gathering general coefficients for the biquad equation
        float omega = juce::MathConstants<float>::twoPi * freq / 44100.0f;  // Angular frequency
        float g = std::tanf(juce::MathConstants<float>::pi * cutoff / 44100.0f);
        float gSq = g * g; // g squared
        float R = 1.0f / (2.0f * resonance);  // Inverse of res because of flipped y

        float a0 = 1.0f + 2.0f * R * g + gSq;
        float a1 = (2.0f * (gSq - 1.0f)) / a0;
        float a2 = (1.0f - 2 * R * g + gSq) / a0;

        auto biquadGeneralSol = [omega, a1, a2](float b0, float b1, float b2) {
            float num1 = b0 + b1 * std::cosf(omega) + b2 * std::cosf(2.0f * omega);
            float num2 = b1 * std::sinf(omega) + b2 * std::sinf(2.0f * omega);
            float num = std::sqrtf(num1 * num1 + num2 * num2);

            float den1 = 1.0f + a1 * std::cosf(omega) + a2 * std::cosf(2.0f * omega);
            float den2 = a1 * std::sinf(omega) + a2 * std::sinf(2.0f * omega);
            float den = std::sqrtf(den1 * den1 + den2 * den2);

            return num / den;
        };

        switch (filterType) {
        case 0: {  // Lowpass
            float b0 = gSq / a0;
            float b1 = 2.0f * gSq / a0;
            float b2 = gSq / a0;
            mag = biquadGeneralSol(b0, b1, b2);
            break;
        }
        case 1: {  // Highpass
            float b0 = 1.0f / a0;
            float b1 = -2.0f / a0;
            float b2 = 1.0f / a0;
            mag = biquadGeneralSol(b0, b1, b2);
            break;
        }
        case 2: {  // Bandpass
            float b0 = g / a0;
            float b1 = 0 / a0;
            float b2 = -g / a0;
            mag = biquadGeneralSol(b0, b1, b2);
            break;
        }
        }

        mag = magToDecibels(mag);
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
    path.lineTo(width, height);
    path.lineTo(0.0f, height);
    path.lineTo(0.0f, height / 2);

    path.closeSubPath();
    GlowStyle::drawRadiantWaveform(g, path, bounds, false, 0.6f);
}

void ItnLookAndFeel::drawSliderValueBox(juce::Graphics& g, juce::Label& label) {
    auto bounds = label.getLocalBounds();

    if (!label.isBeingEdited()) {
        g.setColour(GlowStyle::accentPeach);
        g.setFont(juce::Font(jetBrainsMonoTypeFaceRegular).withHeight(12.0f));
        g.drawText(label.getText(), bounds.toNearestInt(), label.getJustificationType(), true);
    }
}
