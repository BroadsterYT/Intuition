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
        bands.push_back(EQBand());
    }
}

void EqualizerModule::prepare(double sr, int samplesPerBlock, int numChannels) {
    sampleRate = sr;
    for (auto& band : bands) {
        band.prepare(sr, samplesPerBlock, numChannels);
    }
}

void EqualizerModule::updateParameters() {
    auto selectFilterType = [&](int choice) {
        switch (choice) {
        case 0:
            return FilterType::HighPass;
        case 1:
            return FilterType::HighShelf;
        case 2:
            return FilterType::Peaking;
        case 3:
            return FilterType::LowShelf;
        case 4:
            return FilterType::LowPass;
        }
    };

    bands[0].setFilterType(selectFilterType(parameters.getRawParameterValue("EQBAND1_FILTER_TYPE")->load()));
    bands[1].setFilterType(selectFilterType(parameters.getRawParameterValue("EQBAND2_FILTER_TYPE")->load()));
    bands[2].setFilterType(selectFilterType(parameters.getRawParameterValue("EQBAND3_FILTER_TYPE")->load()));
    bands[3].setFilterType(selectFilterType(parameters.getRawParameterValue("EQBAND4_FILTER_TYPE")->load()));
    bands[4].setFilterType(selectFilterType(parameters.getRawParameterValue("EQBAND5_FILTER_TYPE")->load()));
    bands[5].setFilterType(selectFilterType(parameters.getRawParameterValue("EQBAND6_FILTER_TYPE")->load()));
    bands[6].setFilterType(selectFilterType(parameters.getRawParameterValue("EQBAND7_FILTER_TYPE")->load()));
    bands[7].setFilterType(selectFilterType(parameters.getRawParameterValue("EQBAND8_FILTER_TYPE")->load()));

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
}

const EQBand& EqualizerModule::getBand(int bandIndex) {
    return bands[bandIndex];
}