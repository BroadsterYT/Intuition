/*
  ==============================================================================

    ItnFFT.cpp
    Created: 20 Nov 2025 10:33:24pm
    Author:  Brody Brazill

  ==============================================================================
*/

#include "ItnFFT.h"

ItnFFT::ItnFFT() : fft(fftOrder), window(fftSize, juce::dsp::WindowingFunction<float>::hann) {
    fftBuffer.assign(fftSize * 2, 0.0f);
}

std::vector<float> ItnFFT::perform(juce::AudioBuffer<float>& buffer) {
    fftBuffer.assign(fftSize * 2, 0.0f);
    
    for (int i = 0; i < buffer.getNumSamples(); ++i) {
        // Filling buffer with real values
        fftBuffer[2 * i] = buffer.getSample(0, i);  // TODO: L and R channel support
    }

    window.multiplyWithWindowingTable(fftBuffer.data(), fftSize);
    fft.performRealOnlyForwardTransform(fftBuffer.data());

    // Complex bins to magnitudes
    std::vector<float> magnitudes(fftSize / 2);

    for (int bin = 0; bin < fftSize / 2; ++bin) {
        auto real = fftBuffer[2 * bin];
        auto imag = fftBuffer[2 * bin + 1];
        magnitudes[bin] = std::sqrt(real * real + imag * imag);
    }

    return magnitudes;
}
