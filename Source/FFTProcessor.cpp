/*
  ==============================================================================

    FFTProcessor.cpp
    Created: 3 Feb 2026 11:40:45am
    Author:  BroDe

  ==============================================================================
*/

#include "FFTProcessor.h"

FFTProcessor::FFTProcessor() : fft(fftOrder), window(fftSize + 1, juce::dsp::WindowingFunction<float>::WindowingMethod::hann, false) {
    reset();
}

void FFTProcessor::prepare(double sr, int channelNum) {
    sampleRate = sr;
    channel = channelNum;
}

void FFTProcessor::reset() {
    hopCount = 0;
    writePos = 0;
    std::fill(inputFifo.begin(), inputFifo.end(), 0.0f);
    std::fill(outputFifo.begin(), outputFifo.end(), 0.0f);
}

void FFTProcessor::processBlock(juce::AudioBuffer<float>& buffer) {
    int numSamples = buffer.getNumSamples();
    float* channelPtr = buffer.getWritePointer(channel);

    for (int i = 0; i < numSamples; ++i) {
        float sample = processSample(channelPtr[i]);
        channelPtr[i] = sample;
    }
}

float FFTProcessor::processSample(float& sample) {
    inputFifo[writePos] = sample;

    float outputSample = outputFifo[writePos];
    outputFifo[writePos] = 0.0f;

    writePos++;
    if (writePos == fftSize) {
        writePos = 0;
    }

    hopCount++;
    if (hopCount == hopSize) {
        hopCount = 0;
        processFrame();
    }

    return outputSample;
}

void FFTProcessor::processFrame() {
    const float* inputPtr = inputFifo.data();
    float* fftPtr = fftData.data();

    std::memcpy(fftPtr, inputPtr + writePos, (fftSize - writePos) * sizeof(float));
    if (writePos > 0) {
        std::memcpy(fftPtr + fftSize - writePos, inputPtr, writePos * sizeof(float));
    }

    window.multiplyWithWindowingTable(fftPtr, fftSize);

    // if (!bypassed) {
    fft.performRealOnlyForwardTransform(fftPtr, true);
    processSpectrum(fftPtr, numBins);
    fft.performRealOnlyInverseTransform(fftPtr);

    window.multiplyWithWindowingTable(fftPtr, fftSize);
    for (int i = 0; i < fftSize; ++i) {
        fftPtr[i] *= windowCorrection;
    }

    for (int i = 0; i < writePos; ++i) {
        outputFifo[i] += fftData[i + fftSize - writePos];
    }
    for (int i = 0; i < fftSize - writePos; ++i) {
        outputFifo[i + writePos] += fftData[i];
    }
}

void FFTProcessor::processSpectrum(float* data, int numBins) {
    auto* cdata = reinterpret_cast<std::complex<float>*>(data);
    //int cutoffBin = static_cast<int>(2000.0f / sampleRate * fftSize);

    for (int i = 0; i < numBins; ++i) {
        float magnitude = std::abs(cdata[i]);
        float phase = std::arg(cdata[i]);

        // Spectrum manipulation goes here
        //if (i > cutoffBin) magnitude = 0.0f;

        cdata[i] = std::polar(magnitude, phase);
    }
}
