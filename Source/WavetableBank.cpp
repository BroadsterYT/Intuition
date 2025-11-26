/*
  ==============================================================================

    WavetableBank.cpp
    Created: 3 Oct 2025 5:48:18pm
    Author:  Brody

  ==============================================================================
*/

#include "WavetableBank.h"
#include "WavetableHelper.h"


WavetableBank::WavetableBank() {

}

void WavetableBank::addWavetable(juce::File& wavFile) {
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    jassert(wavFile.existsAsFile());
    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(wavFile));

    if (reader != nullptr) {
        juce::AudioBuffer<float> temp;
        temp.setSize((int)reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&temp, 0, (int)reader->lengthInSamples, 0, true, true);

        WavetableHelper::preprocessWavetable(temp);
        WavetableHelper::phaseAlignWavetable(temp);

        WavetableInfo info = { false, wavFile };
        wavetableInfos.push_back(info);
        wavetables.push_back(temp);
    }
}

void WavetableBank::addWavetable(const char* binaryWav, int binarySize) {
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    auto inputStream = std::make_unique<juce::MemoryInputStream>(binaryWav, binarySize, false);

    std::unique_ptr<juce::AudioFormatReader> reader(formatManager.createReaderFor(std::move(inputStream)));

    if (reader.get()) {
        juce::AudioBuffer<float> buffer;
        buffer.setSize((int)reader->numChannels, (int)reader->lengthInSamples);
        reader->read(&buffer,
            0,
            (int)reader->lengthInSamples,
            0,
            true, true
        );

        WavetableHelper::preprocessWavetable(buffer);
        WavetableHelper::phaseAlignWavetable(buffer);

        WavetableInfo info = { true, juce::File{}, WavetableHelper::getWavId(binaryWav) };
        wavetableInfos.push_back(info);
        wavetables.push_back(buffer);
    }
}

void WavetableBank::removeWavetable(int index) {
    wavetableInfos.erase(wavetableInfos.begin() + index);
    wavetables.erase(wavetables.begin() + index);
}

void WavetableBank::updateWavetable(int index, const juce::AudioBuffer<float>& table) {
    wavetables[index].makeCopyOf(table);
}

juce::AudioBuffer<float>& WavetableBank::getWavetable(int index) {
    return wavetables[index];
}

WavetableInfo& WavetableBank::getWavetableInfo(int index) {
    return wavetableInfos[index];
}

void WavetableBank::clear() {
    wavetableInfos.clear();
    wavetables.clear();
}

size_t WavetableBank::size() {
    return wavetables.size();
}