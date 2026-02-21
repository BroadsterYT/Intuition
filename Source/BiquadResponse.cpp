/*
  ==============================================================================

    BiquadResponse.cpp
    Created: 20 Feb 2026 8:42:05pm
    Author:  BroDe

  ==============================================================================
*/

#include "BiquadResponse.h"

float BiquadResponse::magnitudeToDecibels(float mag) {
    return 20.0f * std::log10(mag + 1e-6f);
}

float BiquadResponse::getDecibelsAtFrequency(float freq, float sampleRate, float b0, float b1, float b2, float a1, float a2) {
    float omega = 2.0f * juce::MathConstants<float>::pi * freq / sampleRate;

    std::complex<float> z1 = std::exp(std::complex<float>(0.0f, -omega));
    std::complex<float> z2 = std::exp(std::complex<float>(0.0f, -2.0f * omega));

    std::complex<float> numerator = b0 + b1 * z1 + b2 * z2;
    std::complex<float> denominator = 1.0f + a1 * z1 + a2 * z2;

    std::complex<float> H = numerator / denominator;
    return 20.0f * std::log10(std::abs(H));  // Magnitude to decibel conversion for complex
}
