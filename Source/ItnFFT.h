/*
  ==============================================================================

    ItnFFT.h
    Created: 20 Nov 2025 10:33:24pm
    Author:  Brody Brazill

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <algorithm>


class ItnFFT {
public:
    ItnFFT();

    std::vector<float> perform(juce::AudioBuffer<float>& buffer);

private:
    static constexpr int fftOrder = 10;
    static constexpr int fftSize = 1 << fftOrder;
    
    juce::dsp::FFT fft;
    juce::dsp::WindowingFunction<float> window;

    std::vector<float> fftBuffer;
};