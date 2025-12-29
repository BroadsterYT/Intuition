/*
  ==============================================================================

    TooltipController.cpp
    Created: 29 Dec 2025 1:26:51pm
    Author:  BroDe

  ==============================================================================
*/

#include "TooltipController.h"

TooltipController::TooltipController(juce::Component& target) : owner(target) {
    owner.addMouseListener(this, false);
    startTimerHz(60);

    auto* top = owner.getTopLevelComponent();
    if (!top) top->addChildComponent(tooltip);
}

TooltipController::~TooltipController() {
    owner.removeMouseListener(this);
    stopTimer();
    tooltip.hide();
}

void TooltipController::setTooltipText(const juce::String parameterKey) {
    auto jsonText = juce::String::fromUTF8(BinaryData::tooltips_json, BinaryData::tooltips_jsonSize);
    juce::var parsed = juce::JSON::parse(jsonText);

    if (parsed.isVoid() || parsed.isUndefined()) {
        DBG("Error: Unable to parse JSON file.");
        return;
    }

    auto* obj = parsed.getDynamicObject();
    if (!obj) {
        DBG("Error: Could not convert JSON dat to Dynamic Object.");
        return;
    }

    auto tooltipText = obj->getProperty(parameterKey);
    auto* textObj = tooltipText.getDynamicObject();
    juce::String header = textObj->getProperty("header");
    juce::String subheader = textObj->getProperty("subheader");
    juce::String description = textObj->getProperty("description");

    tooltip.setHeader(header);
    tooltip.setSubheader(subheader);
    tooltip.setDescription(description, true);
}

void TooltipController::mouseEnter(const juce::MouseEvent&) {
    hovering = true;
    hoverFrames = 0;
}

void TooltipController::mouseExit(const juce::MouseEvent&) {
    hovering = false;
    hoverFrames = 0;
    tooltipVisible = false;
    tooltip.hide();
}

void TooltipController::mouseDrag(const juce::MouseEvent&) {
    hovering = false;
    hoverFrames = 0;
    tooltipVisible = false;
    tooltip.hide();
}

void TooltipController::timerCallback() {
    if (!hovering || tooltipVisible) return;

    hoverFrames++;

    if (hoverFrames >= 30) {
        showTooltip();
    }
}

void TooltipController::showTooltip() {
    tooltipVisible = true;

    auto* top = owner.getTopLevelComponent();
    if (!top) return;

    if (tooltip.getParentComponent() != top)
        top->addChildComponent(tooltip);


    auto posInWindow = top->getLocalPoint(&owner, juce::Point<int>(0, 0));

    int x = posInWindow.getX() >= top->getWidth() / 2
        ? posInWindow.getX() - tooltip.getFullTextWidth()
        : posInWindow.getX() + owner.getWidth();
    int y = posInWindow.getY() >= top->getHeight() / 2
        ? posInWindow.getY() - tooltip.getFullTextHeight()
        : posInWindow.getY() + owner.getHeight();

    tooltip.show(x, y);
}
