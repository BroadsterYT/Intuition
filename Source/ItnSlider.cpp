/*
  ==============================================================================

    ItnSlider.cpp
    Created: 25 Sep 2025 6:48:55pm
    Author:  Brody

  ==============================================================================
*/

#include "ItnSlider.h"


void ItnSlider::mouseDown(const juce::MouseEvent& e) {
    if (e.mods.isRightButtonDown()) {
        juce::PopupMenu menu;
        menu.addItem("Set Value Manually...", [this] {
            auto entry = std::make_unique<InlineValueEntry>(getValue());
            entry->linkToSlider(this);
            entry->setSize(50, 25);

            juce::CallOutBox::launchAsynchronously(
                std::move(entry),
                getScreenBounds(),
                nullptr
            );
        });
        menu.addItem("Reset to Default", [this] {/*NYI*/});

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this).withMousePosition());
    }
    else {
        juce::Slider::mouseDown(e);
    }
}