/*
  ==============================================================================

    ReverbModule.cpp
    Created: 30 Oct 2025 5:35:58pm
    Author:  Brody

  ==============================================================================
*/

#include "ReverbModule.h"

ReverbModule::ReverbModule(
    juce::AudioProcessorValueTreeState& vts,
    ModMatrix* modMatrix
) : parameters(vts),
    modMatrix(modMatrix) {
    reverbParams.damping = 0.5f;
    reverbParams.roomSize = 0.5f;
    reverbParams.width = 1.0f;
    reverbParams.dryLevel = 0.7f;
    reverbParams.wetLevel = 0.3f;
}

void ReverbModule::prepare(double sampleRate, int samplesPerBlock, int numChannels) {
    if (preparedChannels != numChannels) {
        juce::dsp::ProcessSpec pspec;
        pspec.sampleRate = sampleRate;
        pspec.maximumBlockSize = samplesPerBlock;
        pspec.numChannels = numChannels;
        reverb.prepare(pspec);

        preparedChannels = numChannels;
    }
}

void ReverbModule::updateParameters() {
    reverbParams.damping = modMatrix->getModdedDest("REVERB_DAMPING");
    reverbParams.roomSize = modMatrix->getModdedDest("REVERB_ROOM_SIZE");
    reverbParams.width = modMatrix->getModdedDest("REVERB_WIDTH");
    reverbParams.dryLevel = modMatrix->getModdedDest("REVERB_DRY_LEVEL");
    reverbParams.wetLevel = modMatrix->getModdedDest("REVERB_WET_LEVEL");
    reverb.setParameters(reverbParams);
}

void ReverbModule::process(juce::AudioBuffer<float>& buffer) {
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    reverb.process(context);
}
