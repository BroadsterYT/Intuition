/*
  ==============================================================================

    ItnContext.cpp
    Created: 23 Oct 2025 8:42:07pm
    Author:  BroDe

  ==============================================================================
*/

#include "ItnContext.h"

ItnContext::ItnContext(
    juce::AudioProcessor* ap,
    juce::AudioProcessorValueTreeState& vts,
    ModMatrix* modMatrix,
    EnvelopeManager& envManager,
    WavetableBank& bank1, 
    WavetableBank& bank2,
    WavetableBank& bank3,
    WavetableBank& bank4,
    LFOShape& shape1,
    LFOShape& shape2,
    LFOShape& shape3,
    float* phase1,
    float* phase2,
    float* phase3,
    EqualizerModule& equalizer
) : ap(ap),
    parameters(vts),
    modMatrix(modMatrix),
    envManager(envManager),
    
    bank1(bank1),
    bank2(bank2),
    bank3(bank3),
    bank4(bank4),
    
    shape1(shape1),
    shape2(shape2),
    shape3(shape3),
    phase1(phase1),
    phase2(phase2),
    phase3(phase3),

    equalizer(equalizer) {}
