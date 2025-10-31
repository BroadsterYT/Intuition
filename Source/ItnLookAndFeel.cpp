/*
  ==============================================================================

    ItnLookAndFeel.cpp
    Created: 29 Sep 2025 1:08:01am
    Author:  BroDe

  ==============================================================================
*/

#include "ItnLookAndFeel.h"


juce::Slider::SliderLayout ItnLookAndFeel::getSliderLayout(juce::Slider& slider) {
    juce::Slider::SliderLayout layout;

    auto bounds = slider.getLocalBounds();
    layout.sliderBounds = bounds;

    if (slider.getTextBoxPosition() == juce::Slider::TextBoxAbove) {
        auto textArea = bounds.removeFromTop(slider.getTextBoxHeight());
        auto labelArea = bounds.removeFromBottom(10);
        float textBoxPadding = bounds.getWidth() - slider.getTextBoxWidth();
        textArea.removeFromLeft(textBoxPadding / 2);
        textArea.removeFromRight(textBoxPadding / 2);

        layout.sliderBounds = bounds;
        layout.textBoxBounds = textArea;
    }
    else if (slider.getTextBoxPosition() == juce::Slider::NoTextBox) {
        auto labelArea = bounds.removeFromBottom(10);
        slider.setPopupDisplayEnabled(true, false, slider.getParentComponent());

        layout.sliderBounds = bounds;
    }

    return layout;
}

void ItnLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) {
    auto radius = juce::jmin(width / 2.0f, height / 2.0f) - 8.0f;
    auto centerX = x + width * 0.5f;
    auto centerY = y + height * 0.5f;

    // --- Draw background path (unfilled arc) ---
    g.setColour(GlowStyle::shadow);  // background arc color
    juce::Path baseArc;

    baseArc.addCentredArc(centerX, centerY, radius, radius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
    g.strokePath(baseArc, juce::PathStrokeType(8.0f));

    /*juce::Path fillArc;
    fillArc.addCentredArc(centerX, centerY, radius, radius, 0.0f, rotaryStartAngle, juce::jmap(sliderPosProportional, rotaryStartAngle, rotaryEndAngle), true);
    g.setColour(GlowStyle::warmHighlight);
    g.strokePath(fillArc, juce::PathStrokeType(8.0f));*/
    float angle = 3.0f * juce::MathConstants<float>::pi / 2.0f + rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    float knobRadius = 6.0f;
    float knobX = centerX + std::cos(angle) * radius - knobRadius;
    float knobY = centerY + std::sin(angle) * radius - knobRadius;

    g.setColour(GlowStyle::warmHighlight);
    g.fillEllipse(knobX, knobY, knobRadius * 2.0f, knobRadius * 2.0f);
}

void ItnLookAndFeel::drawWaveform(
    juce::Graphics& g,
    juce::Rectangle<float> bounds,
    const juce::Array<float> waveform
) {
    int width = bounds.getWidth();
    int height = bounds.getHeight();
    
    juce::Path fill, line;
    fill.startNewSubPath(0, height / 2);
    line.startNewSubPath(0, height / 2);

    for (int i = 0; i < waveform.size(); ++i) {
        float x = juce::jmap((float)i, 0.0f, (float)waveform.size(), 0.0f, (float)width);
        float y = juce::jmap(waveform[i], -1.0f, 1.0f, (float)height, 0.0f);
        fill.lineTo(x, y);
        line.lineTo(x, y);
    }
    fill.lineTo(width, height / 2);
    line.lineTo(width, height / 2);

    fill.closeSubPath();
    GlowStyle::drawRadiantWaveform(g, fill, bounds, true);
    //GlowStyle::drawFilamentWaveform(g, line, bounds);
}

void ItnLookAndFeel::drawEnvelope(
    juce::Graphics& g,
    juce::Rectangle<float> bounds,
    float attack,
    float decay,
    float sustain,
    float release,
    float envTime
) {
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

        switch (filterType) {
        case 0:  // Lowpass
            mag = 1.0f / std::sqrt(1.0f + std::pow(freq / cutoff, 2.0f) * (1.0f + resonance));
            break;
        case 1:  // Highpass
            mag = 1.0f / std::sqrt(1.0f + std::pow(cutoff / freq, 2.0f) * (1.0f + resonance));
            break;
        case 2: { // Bandpass
            float Q = 1.0f + resonance * 19.0f;
            float ratio = freq / cutoff;
            mag = 1.0f / std::sqrt(1.0f + Q * Q * (ratio - 1.0f / ratio) * (ratio - 1.0f / ratio));
            break;
        }
        }

        mag = magToDecibels(mag);
        float minDb = -24.0f;
        float maxDb = 24.0f;
        float y = (1.0f - (mag - minDb) / (maxDb - minDb)) * height;
        //DBG("Look: " << y);

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
    GlowStyle::drawRadiantWaveform(g, path, bounds);
}
