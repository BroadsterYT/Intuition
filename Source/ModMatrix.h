/*
  ==============================================================================

    ModMatrix.h
    Created: 15 Oct 2025 11:24:09pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ModSource.h"
#include "ModDestination.h"
#include "ModConnection.h"


class ModMatrix {
public:
    ModMatrix();
    /// <summary>
    /// Apply all modifications to all destinations
    /// </summary>
    void applyMods();
    /// <summary>
    /// Given a ModDestination's name, returns its current modified value
    /// </summary>
    /// <param name="destName">The name of the destination</param>
    /// <returns>The modified value of the destination's parameter</returns>
    float getModdedDest(const juce::String destName);

    /// <summary>
    /// Retrieves the names of all available mod sources and places them into a vector
    /// </summary>
    /// <param name="output">The vector to place all source names into</param>
    void getAllSourceNames(std::vector<juce::String>& output);
    /// <summary>
    /// Returns a pointer to a ModSource given its name
    /// </summary>
    /// <param name="sourceName">The name of the ModSource</param>
    /// <returns>A pointer to the ModSource object</returns>
    ModSource* getSource(const juce::String sourceName);
    /// <summary>
    /// Adds a new mod source to the matrix and retuns it
    /// </summary>
    /// <param name="sourceName">The name to give to the new source</param>
    /// <returns>The newly created ModSource, returns nullptr if ModSource could not be added</returns>
    ModSource* addSource(const juce::String sourceName);
    ModDestination* getDestination(const juce::String destName);
    /// <summary>
    /// Adds a new mod destination to the matrix and returns it
    /// </summary>
    /// <param name="destName">The name to give to the new destination</param>
    /// <returns>The newly added ModDestination, nullptr if ModDestination could not be added</returns>
    ModDestination* addDestination(const juce::String destName);
    ModConnection* getConnection(const juce::String sourceName, const juce::String destName);
    
    void addConnection(const juce::String sourceName, const juce::String destName);
    void removeConnection(const juce::String sourceName, const juce::String destName);

private:
    std::unordered_map<juce::String, std::unique_ptr<ModSource>> sources;
    std::unordered_map<juce::String, std::unique_ptr<ModDestination>> destinations;
    std::vector<std::unique_ptr<ModConnection>> connections;
};