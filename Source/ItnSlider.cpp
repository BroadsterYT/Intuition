/*
  ==============================================================================

    ItnSlider.cpp
    Created: 25 Sep 2025 6:48:55pm
    Author:  Brody

  ==============================================================================
*/

#include "ItnSlider.h"


ItnSlider::ItnSlider() {
    setLookAndFeel(&lookAndFeel);
    setSliderStyle(juce::Slider::Rotary);
    setTextBoxStyle(juce::Slider::TextBoxAbove, false, 40, 15);

    addAndMakeVisible(label);
    label.setJustificationType(juce::Justification::centredBottom);
    updateLabel();
}

ItnSlider::~ItnSlider() {
    setLookAndFeel(nullptr);
}

void ItnSlider::setLabelNames(juce::String newFullName, juce::String newNickname) {
    fullName = newFullName;
    nickname = newNickname;
    updateLabel();
}

void ItnSlider::updateLabel() {
    label.setText(nickname, juce::dontSendNotification);
    setTooltip(fullName);
}

void ItnSlider::mouseDown(const juce::MouseEvent& e) {
    if (e.mods.isRightButtonDown()) {
        juce::PopupMenu menu;
        menu.addItem("Set Value Manually...", [this] {
            auto entry = std::make_unique<InlineValueEntry<double>>(getValue());
            entry->linkToComponent<ItnSlider>(this, [](ItnSlider* s, double val) {
                s->setValue(val, juce::sendNotification);
            });
            entry->setSize(50, 25);

            juce::CallOutBox::launchAsynchronously(
                std::move(entry),
                getScreenBounds(),
                nullptr
            );
        });
        if (canBeModded) addModLinkSubmenu(menu);

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this).withMousePosition());
    }
    else {
        juce::Slider::mouseDown(e);
    }
}

void ItnSlider::resized() {
    juce::Slider::resized();
    auto area = getLocalBounds().removeFromBottom(15);

    label.setBounds(area);
}

void ItnSlider::setModMatrix(ModMatrix* matrix) {
    modMatrix = matrix;
}

void ItnSlider::addModLinkSubmenu(juce::PopupMenu& menu) {
    menu.addSeparator();
    menu.addSubMenu("Link to Mod Source...", [] {
        juce::PopupMenu sub;
        sub.addItem("LFO 1", [] {/*NYI*/});
        return sub;
    }());
}
