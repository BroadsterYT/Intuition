/*
  ==============================================================================

    EqualizerModule.cpp
    Created: 18 Feb 2026 4:14:56pm
    Author:  BroDe

  ==============================================================================
*/

#include "EqualizerModule.h"


EqualizerModule::EqualizerModule(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix) : parameters(vts), modMatrix(modMatrix) {
    for (int i = 0; i < 8; ++i) {
        bands.push_back(EQBand(i));
    }
}

void EqualizerModule::prepare(double sr, int samplesPerBlock, int numChannels) {
    sampleRate = sr;
    for (auto& band : bands) {
        band.prepare(sr, samplesPerBlock, numChannels);
    }

    fft[0].prepare(sampleRate, 0);
    fft[1].prepare(sampleRate, 1);
}

void EqualizerModule::updateParameters() {
    setBandFilterType(0, "EQBAND1_FILTER_TYPE");
    setBandFilterType(1, "EQBAND2_FILTER_TYPE");
    setBandFilterType(2, "EQBAND3_FILTER_TYPE");
    setBandFilterType(3, "EQBAND4_FILTER_TYPE");
    setBandFilterType(4, "EQBAND5_FILTER_TYPE");
    setBandFilterType(5, "EQBAND6_FILTER_TYPE");
    setBandFilterType(6, "EQBAND7_FILTER_TYPE");
    setBandFilterType(7, "EQBAND8_FILTER_TYPE");

    bands[0].setFrequency(modMatrix->getModdedDest("EQBAND1_FREQUENCY"));
    bands[1].setFrequency(modMatrix->getModdedDest("EQBAND2_FREQUENCY"));
    bands[2].setFrequency(modMatrix->getModdedDest("EQBAND3_FREQUENCY"));
    bands[3].setFrequency(modMatrix->getModdedDest("EQBAND4_FREQUENCY"));
    bands[4].setFrequency(modMatrix->getModdedDest("EQBAND5_FREQUENCY"));
    bands[5].setFrequency(modMatrix->getModdedDest("EQBAND6_FREQUENCY"));
    bands[6].setFrequency(modMatrix->getModdedDest("EQBAND7_FREQUENCY"));
    bands[7].setFrequency(modMatrix->getModdedDest("EQBAND8_FREQUENCY"));

    bands[0].setGain(modMatrix->getModdedDest("EQBAND1_GAIN"));
    bands[1].setGain(modMatrix->getModdedDest("EQBAND2_GAIN"));
    bands[2].setGain(modMatrix->getModdedDest("EQBAND3_GAIN"));
    bands[3].setGain(modMatrix->getModdedDest("EQBAND4_GAIN"));
    bands[4].setGain(modMatrix->getModdedDest("EQBAND5_GAIN"));
    bands[5].setGain(modMatrix->getModdedDest("EQBAND6_GAIN"));
    bands[6].setGain(modMatrix->getModdedDest("EQBAND7_GAIN"));
    bands[7].setGain(modMatrix->getModdedDest("EQBAND8_GAIN"));

    bands[0].setQuality(modMatrix->getModdedDest("EQBAND1_Q"));
    bands[1].setQuality(modMatrix->getModdedDest("EQBAND2_Q"));
    bands[2].setQuality(modMatrix->getModdedDest("EQBAND3_Q"));
    bands[3].setQuality(modMatrix->getModdedDest("EQBAND4_Q"));
    bands[4].setQuality(modMatrix->getModdedDest("EQBAND5_Q"));
    bands[5].setQuality(modMatrix->getModdedDest("EQBAND6_Q"));
    bands[6].setQuality(modMatrix->getModdedDest("EQBAND7_Q"));
    bands[7].setQuality(modMatrix->getModdedDest("EQBAND8_Q"));
}

void EqualizerModule::processBlock(juce::AudioBuffer<float>& buffer) {
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    for (auto& band : bands) {
        band.updateCoefficients();
        band.process(context);
    }
    fft[0].processBlock(buffer);
    fft[1].processBlock(buffer);
}

const EQBand& EqualizerModule::getBand(int bandIndex) {
    return bands[bandIndex];
}

bool EqualizerModule::getFFTData(std::vector<float>& output, int channelNum) {
    return fft[channelNum].getFFTData(output);
}

void EqualizerModule::setBandFilterType(int bandIndex, const juce::String& filterParamName) {
    int choice = (int)parameters.getRawParameterValue(filterParamName)->load();
    switch (choice) {
    case (int)FilterType::HighPass:
        bands[bandIndex].setFilterType(FilterType::HighPass);
        break;
    case (int)FilterType::HighShelf:
        bands[bandIndex].setFilterType(FilterType::HighShelf);
        break;
    case (int)FilterType::Peaking:
        bands[bandIndex].setFilterType(FilterType::Peaking);
        break;
    case (int)FilterType::LowShelf:
        bands[bandIndex].setFilterType(FilterType::LowShelf);
        break;
    case (int)FilterType::LowPass:
        bands[bandIndex].setFilterType(FilterType::LowPass);
        break;
    default:
        break;
    }
}
