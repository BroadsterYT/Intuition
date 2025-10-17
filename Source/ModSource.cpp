/*
  ==============================================================================

    ModSource.cpp
    Created: 15 Oct 2025 11:23:24pm
    Author:  BroDe

  ==============================================================================
*/

#include "ModSource.h"


ModSource::ModSource(const juce::String name, float* value) : name(name), value(value) {

}

juce::String ModSource::getName() const {
    return name;
}

float ModSource::getValue() const {
    if (!value) {
        DBG("ModSource value error: referenced value is nullptr.");
        return 0.0f;
    }
    return *value;
}

void ModSource::setValuePtr(float* newVal) {
    value = newVal;
}
