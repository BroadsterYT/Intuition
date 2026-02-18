/*
  ==============================================================================

    FFTProcessor.h
    Created: 3 Feb 2026 11:40:45am
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <complex>


class FFTProcessor {
public:
    FFTProcessor();

    void prepare(double sr, int channelNum);
    void reset();

    /// <summary>
    /// Performs the FFT/inverse FFT along with any frequency modifications and places the results back into buffer
    /// </summary>
    /// <param name="buffer">The audio data to pass through the FFT</param>
    void processBlock(juce::AudioBuffer<float>& buffer);

private:
    static constexpr int fftOrder = 10;
    static constexpr int fftSize = 1 << fftOrder;
    static constexpr int numBins = fftSize / 2 + 1;
    static constexpr int overlap = 4;
    static constexpr int hopSize = fftSize / overlap;
    static constexpr float windowCorrection = 2.0f / 3.0f;

    double sampleRate = 44100.0;
    int channel = 0;  // The channel to read from the audio buffer

    juce::dsp::FFT fft;
    juce::dsp::WindowingFunction<float> window;

    int hopCount = 0;  // Counts up to hopSize
    int writePos = 0;  // Keeps track of current write position in input FIFO and output FIFO
    std::array<float, fftSize> inputFifo;
    std::array<float, fftSize> outputFifo;
    std::array<float, fftSize * 2> fftData; // Holds real, imag pairs so need double size

    float processSample(float& sample);
    void processFrame();
    virtual void processSpectrum(float* data);
};