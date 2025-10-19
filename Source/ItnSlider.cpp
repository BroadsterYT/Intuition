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

void ItnSlider::setLabelNames(const juce::String newFullName, const juce::String newNickname) {
    fullName = newFullName;
    nickname = newNickname;
    updateLabel();
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
        addModLinkSubmenu(menu);

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

void ItnSlider::setModMatrix(ModMatrix* matrix, const juce::String pName) {
    modMatrix = matrix;
    paramName = pName;
}

void ItnSlider::updateLabel() {
    label.setText(nickname, juce::dontSendNotification);
    setTooltip(fullName);
}

void ItnSlider::addModLinkSubmenu(juce::PopupMenu& menu) {
    if (!modMatrix || paramName == "") return;
    menu.addSeparator();

    juce::PopupMenu sub;

    std::vector<juce::String> sourceNames;
    modMatrix->getAllSourceNames(sourceNames);
    for (auto& source : sourceNames) {
        if (modMatrix->getConnection(source, paramName)) {
            addModLinkPropertiesSubmenu(sub, source);
        }
        else {
            sub.addItem(source, [this, source] {
                modMatrix->addConnection(source, paramName);
            });
        }
    }

    menu.addSubMenu("Link to Mod Source...", sub);
}

void ItnSlider::addModLinkPropertiesSubmenu(juce::PopupMenu& menu, const juce::String sourceName) {
    auto* conn = modMatrix->getConnection(sourceName, paramName);
    if (!conn) return;

    juce::PopupMenu sub;

    sub.addItem("Unlink mod source", [this, sourceName] {
        modMatrix->removeConnection(sourceName, paramName);
    });
    sub.addItem("Active", true, conn->getActive(), [conn] {
        if (conn->getActive()) conn->setActive(false);
        else conn->setActive(true);
    });
    sub.addItem("Bipolar", true, conn->getBipolar(), [conn] {
        if (conn->getBipolar()) conn->setBipolar(false);
        else conn->setBipolar(true);
    });
    sub.addItem("Set mod opacity...", [] {/*NYI*/});
    sub.addItem("Set mod depth...", [] {/*NYI*/});

    menu.addSubMenu(sourceName, sub);
}