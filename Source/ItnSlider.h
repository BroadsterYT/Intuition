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


class ItnSlider : public juce::Slider, private juce::Timer {
public:
    ItnSlider();
    ~ItnSlider();

    /// <summary>
    /// Sets the display names of the slider.
    /// </summary>
    /// <param name="newFullName">The name of the slider to show when hovering over it</param>
    /// <param name="newNickname">The name to display on the slider itself when the slider's label is visible</param>
    void setLabelNames(const juce::String newFullName, const juce::String newNickname);

    void mouseDown(const juce::MouseEvent& e) override;
    void paint(juce::Graphics& g) override;
    void resized() override;

    void setModMatrix(ModMatrix* matrix, juce::String pName);

private:
    ItnLookAndFeel lookAndFeel;
    juce::String fullName;
    juce::String nickname;

    juce::Label label;

    ModMatrix* modMatrix = nullptr;
    juce::String paramName = "";

    void timerCallback() override;

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
    /// <param name="menu">The submemu to add this submenu to</param>
    /// <param name="sourceName">The name of the source involvedin this mod connection</param>
    void addModLinkPropertiesSubmenu(juce::PopupMenu& menu, const juce::String sourceName);
};