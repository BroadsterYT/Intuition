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
#include "ItnTooltip.h"


class ItnSlider : public juce::Slider, private juce::Timer {
public:
    ItnSlider();
    ~ItnSlider();

    void parentHierarchyChanged() override;

    /// <summary>
    /// Sets the display name of the slider.
    /// </summary>
    /// <param name="newName">The name to display on the slider itself when the slider's label is visible</param>
    void setLabelName(const juce::String newName);
    /// <summary>
    /// Sets the text fields for this tooltip, taken from the JSON-formatted tooltip list
    /// </summary>
    /// <param name="parameterKey">The key to extract tooltip information from within the JSON tooltip structure</param>
    void setCustomTooltipText(const juce::String parameterKey);

    void mouseDown(const juce::MouseEvent& e) override;
    void mouseEnter(const juce::MouseEvent& e) override;
    void mouseExit(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;

    void setModMatrix(ModMatrix* matrix, juce::String pName);

private:
    ModMatrix* modMatrix = nullptr;
    juce::String paramName = "";

    // Hover elements
    bool hovering = false;
    ItnTooltip tooltip;
    int tooltipSpawnTimer = 0;
    bool tooltipVisible = false;

    void updateLabel();
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

    void timerCallback() override;
};