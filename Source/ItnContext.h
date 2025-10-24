/*
  ==============================================================================

    ItnContext.h
    Created: 23 Oct 2025 8:42:07pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include "LFOShape.h"
#include "ModMatrix.h"
#include "WavetableBank.h"


/// <summary>
/// Intuition context. Includes a reference to all oscillators, parameters, and
/// other important components needed for GUIs and parameter inputs
/// </summary>
class ItnContext {
public:
    ItnContext(
        juce::AudioProcessor* ap,
        juce::AudioProcessorValueTreeState& vts,
        ModMatrix* modMatrix,

        WavetableBank& bank1,
        WavetableBank& bank2,
        WavetableBank& bank3,
        WavetableBank& bank4,

        LFOShape& shape1,
        LFOShape& shape2,
        LFOShape& shape3,
        float* phase1,
        float* phase2,
        float* phase3
    );

    juce::AudioProcessor* ap;
    juce::AudioProcessorValueTreeState& parameters;
    ModMatrix* modMatrix;

    WavetableBank& bank1;
    WavetableBank& bank2;
    WavetableBank& bank3;
    WavetableBank& bank4;

    LFOShape& shape1;
    LFOShape& shape2;
    LFOShape& shape3;
    float* phase1;
    float* phase2;
    float* phase3;
};