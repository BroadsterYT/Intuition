/*
  ==============================================================================

    ItnTabbedComponent.cpp
    Created: 22 Dec 2025 8:08:00pm
    Author:  BroDe

  ==============================================================================
*/

#include "ItnTabbedComponent.h"

ItnTabbedComponent::ItnTabbedComponent(const juce::TabbedButtonBar::Orientation orientation) : juce::TabbedComponent(orientation) {
    setTabBarDepth(32);
    setOutline(0);

    setColour(juce::TabbedComponent::outlineColourId, juce::Colours::transparentBlack);
    setColour(juce::TabbedComponent::backgroundColourId, juce::Colours::transparentBlack);
}
