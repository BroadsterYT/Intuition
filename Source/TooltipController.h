/*
  ==============================================================================

    TooltipController.h
    Created: 29 Dec 2025 1:26:51pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "ItnTooltip.h"


class TooltipController : private juce::Timer, private juce::MouseListener {
public:
    TooltipController(juce::Component& target);
    ~TooltipController() override;

    void setTooltipText(const juce::String parameterKey);

private:
    juce::Component& owner;
    ItnTooltip tooltip;

    bool hovering = false;
    bool tooltipVisible = false;
    int hoverFrames = 0;

    void mouseEnter(const juce::MouseEvent&) override;
    void mouseExit(const juce::MouseEvent&) override;
    void mouseDrag(const juce::MouseEvent&) override;

    void timerCallback() override;

    void showTooltip();
};