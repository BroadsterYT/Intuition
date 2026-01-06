/*
  ==============================================================================

    ItnSlider.h
    Created: 25 Sep 2025 6:48:43pm
    Author:  Brody

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "InlineValueEntry.h"
#include "ItnLookAndFeel.h"
#include "ModMatrix.h"
#include "TooltipController.h"


class ItnSlider : public juce::Slider {
public:
    ItnSlider(const juce::String tooltipKey);
    ~ItnSlider();

    void mouseDown(const juce::MouseEvent& e) override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;

    void setModMatrix(ModMatrix* matrix, juce::String pName);

private:
    ModMatrix* modMatrix = nullptr;
    juce::String paramName = "";

    TooltipController tooltipController;

    /// <summary>
    /// If this slider supports mods, adds a submenu to the right-click
    /// menu to toggle mod sources
    /// </summary>
    /// <param name="menu">The popup menu to add the submenu to</param>
    void addModLinkSubmenu(juce::PopupMenu& menu);
    /// <summary>
    /// If a mod connection has been established, will create a submenu for editing the properties
    /// of the link
    /// </summary>
    /// <param name="menu">The submenu to add this submenu to</param>
    /// <param name="sourceName">The name of the source involvedin this mod connection</param>
    void addModLinkPropertiesSubmenu(juce::PopupMenu& menu, const juce::String sourceName);
};