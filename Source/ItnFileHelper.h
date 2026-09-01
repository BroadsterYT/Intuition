/*
  ==============================================================================

    ItnFileHelper.h
    Created: 27 Jul 2026 11:21:29am
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class ItnFileHelper {
public:
    /// <summary>
    /// Returns the directory used as the home for all Intuition-related
    /// information, including logs, presets, etc.
    /// </summary>
    /// <returns>Home directory of Intuition</returns>
    static juce::File getItnHomeDirectory();
    /// <summary>
    /// Creates and configures the Intuition home directory,
    /// if it doesn't already exist
    /// </summary>
    static void configureItnHomeDirectory();
};