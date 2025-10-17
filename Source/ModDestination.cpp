/*
  ==============================================================================

    ModDestination.cpp
    Created: 15 Oct 2025 11:23:42pm
    Author:  BroDe

  ==============================================================================
*/

#include "ModDestination.h"


ModDestination::ModDestination(const juce::String name, std::atomic<float>* baseValue) : name(name), baseValue(baseValue) {

}

juce::String ModDestination::getName() {
    return name;
}

float ModDestination::getMinRange() const {
    return minRange;
}

void ModDestination::setMinRange(float min) {
    minRange = min;
}

float ModDestination::getMaxRange() const {
    return maxRange;
}

void ModDestination::setMaxRange(float max) {
    maxRange = max;
}

float ModDestination::getBaseValue() {
    if (!baseValue) {
        DBG("ModDestination error: baseValue is nullptr");
        return 0.0f;
    }
    return baseValue->load();
}

void ModDestination::setBasePtr(std::atomic<float>* base) {
    baseValue = base;
}

float ModDestination::getModdedValue() {
    return moddedValue;
}

void ModDestination::setModdedValue(float newVal) {
    moddedValue = newVal;
}