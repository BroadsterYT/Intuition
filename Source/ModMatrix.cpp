/*
  ==============================================================================

    ModMatrix.cpp
    Created: 15 Oct 2025 11:24:09pm
    Author:  BroDe

  ==============================================================================
*/

#include "ModMatrix.h"


ModMatrix::ModMatrix() {
}

void ModMatrix::applyMods() {
    // Clear existing mods
    for (auto& [name, d] : destinations) {
        if (!d) {
            DBG("Unreferenced destination (" << name << ") found when applying mods. Skipping...");
            continue;
        }
        float baseVal = d->getBaseValue();
        d->setModdedValue(baseVal);
    }

    // Apply new mods
    for (auto& c : connections) {
        ModDestination* dest = c->getDestination();
        float destMin = dest->getMinRange();
        float destMax = dest->getMaxRange();
        
        float src = c->getSource()->getValue();
        float modAmount = 0.0f;

        if (c->getBipolar()) {
            /*float mod = src * c->getDepth() - 0.5f;
            modAmount = juce::jmap(mod, destMin, destMax);*/
            float bipolar = (src - 0.5f) * 2.0f;
            float mod = bipolar * c->getDepth();
            float range = destMax - destMin;

            modAmount = mod * (range * 0.5f);
        }
        else {
            float mod = src * c->getDepth();
            modAmount = juce::jmap(mod, 0.0f, 1.0f, destMin, destMax) - destMin;
        }
        
        float currentDestVal = dest->getModdedValue();
        float unclamped = currentDestVal + modAmount;

        // DBG("Current: " << currentDestVal << "\nMod: " << modAmount);
        
        // TODO: Optimize 
        if (!c->getActive()) unclamped = 0.0f;
        dest->setModdedValue(juce::jlimit(destMin, destMax, unclamped));
    }
}

float ModMatrix::getModdedDest(const juce::String destName) {
    float value = destinations[destName]->getModdedValue();
    return value;
}

void ModMatrix::getAllSourceNames(std::vector<juce::String>& output) {
    for (auto& [name, source] : sources) {
        output.push_back(name);
    }
}

ModSource* ModMatrix::getSource(const juce::String sourceName) {
    return sources[sourceName].get();
}

ModSource* ModMatrix::addSource(const juce::String sourceName) {
    sources.try_emplace(sourceName, std::make_unique<ModSource>(sourceName));
    return getSource(sourceName);
}

ModDestination* ModMatrix::getDestination(const juce::String destName) {
    return destinations[destName].get();
}

ModDestination* ModMatrix::addDestination(const juce::String destName) {
    destinations.try_emplace(destName, std::make_unique<ModDestination>(destName));
    return getDestination(destName);
}

ModConnection* ModMatrix::getConnection(const juce::String sourceName, const juce::String destName) {
    auto it = std::find_if(connections.begin(), connections.end(), 
        [&](std::unique_ptr<ModConnection>& c) {
            return c->getSource()->getName() == sourceName && c->getDestination()->getName() == destName;
        }
    );

    if (it != connections.end()) {
        return it->get();
    }
    else {
        DBG("ModMatrix error: ModConnection not found within ModMatrix");
        return nullptr;
    }
}

void ModMatrix::addConnection(const juce::String sourceName, const juce::String destName) {
    if (sources.find(sourceName) == sources.end()) {
        DBG("ModMatrix error: Source not found within ModMatrix");
        return;
    }
    if (destinations.find(destName) == destinations.end()) {
        DBG("ModMatrix error: Destination not found within ModMatrix");
        return;
    }

    ModSource* source = sources[sourceName].get();
    ModDestination* dest = destinations[destName].get();
    connections.emplace_back(std::make_unique<ModConnection>(source, dest));
    DBG("ModConnection successfully established between " << sourceName << " and " << destName);
}

void ModMatrix::removeConnection(const juce::String sourceName, const juce::String destName) {
    auto it = std::find_if(connections.begin(), connections.end(),
        [&](std::unique_ptr<ModConnection>& c) {
            return c->getSource()->getName() == sourceName && c->getDestination()->getName() == destName;
        }
    );

    if (it != connections.end()) {
        connections.erase(it);
        DBG("Connection successfully removed.");
    }
    else {
        DBG("ModMatrix error: ModConnection not found within ModMatrix");
    }
}
