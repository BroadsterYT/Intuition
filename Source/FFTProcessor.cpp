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
    fftDecibelBuffer.resize(numBins);
    fftDecibelBufferBack.resize(numBins);
}

void FFTProcessor::processBlock(juce::AudioBuffer<float>& buffer) {
    int numSamples = buffer.getNumSamples();
    float* channelPtr = buffer.getWritePointer(channel);

    for (int i = 0; i < numSamples; ++i) {
        processSample(channelPtr[i]);
    }
}

bool FFTProcessor::getFFTData(std::vector<float>& output) {
    if (newDataAvailable.load(std::memory_order_acquire)) {
        output = fftDecibelBuffer;
        newDataAvailable.store(false);
        return true;
    }
    return false;
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

    auto* cdata = reinterpret_cast<std::complex<float>*>(fftPtr);
    for (int i = 0; i < numBins; ++i) {
        float normMag = (std::abs(cdata[i]) / fftSize) * (1.0f / 0.5f) / std::sqrt(2.0f);


        float decibels = juce::Decibels::gainToDecibels(normMag);
        fftDecibelBufferBack[i] = decibels;
    }

    fftDecibelBuffer.swap(fftDecibelBufferBack);
    newDataAvailable.store(true, std::memory_order_release);
}

void FFTProcessor::processSpectrum(float* data) {
    auto* cdata = reinterpret_cast<std::complex<float>*>(data);

    for (int i = 0; i < numBins; ++i) {
        float magnitude = std::abs(cdata[i]);
        float phase = std::arg(cdata[i]);

        cdata[i] = std::polar(magnitude, phase);
    }
}