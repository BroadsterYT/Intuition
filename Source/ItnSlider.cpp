/*
  ==============================================================================

    ItnSlider.cpp
    Created: 25 Sep 2025 6:48:55pm
    Author:  Brody

  ==============================================================================
*/

#include "ItnSlider.h"


ItnSlider::ItnSlider(const juce::String tooltipKey) : tooltipController(*this) {
    setLookAndFeel(&ItnLookAndFeel::getInstance());
    setSliderStyle(juce::Slider::Rotary);
    setTextBoxStyle(juce::Slider::NoTextBox, false, 40, 15);
    tooltipController.setTooltipText(tooltipKey);
}

ItnSlider::~ItnSlider() {
    setLookAndFeel(nullptr);
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

void ItnSlider::paint(juce::Graphics& g) {
    if (!modMatrix || paramName.isEmpty()) {
        juce::Slider::paint(g);
        return;
    }
    if (std::abs(modMatrix->getModdedDest(paramName) - modMatrix->getDestination(paramName)->getBaseValue()) < 0.01f) {
        juce::Slider::paint(g);
        return;
    }

    // Now overlay the modulation indicator
    auto layout = getLookAndFeel().getSliderLayout(*this);
    auto bounds = layout.sliderBounds.reduced(2).toFloat();

    float size = juce::jmin(bounds.getWidth(), bounds.getHeight()) - 6.0f;
    auto center = bounds.getCentre();
    float radius = size / 2.0f;

    // Get values
    auto* dest = modMatrix->getDestination(paramName);
    float baseRaw = dest->getBaseValue();
    float modRaw = modMatrix->getModdedDest(paramName);

    float baseMin = dest->getMinRange();
    float baseMax = dest->getMaxRange();
    float baseVal = (baseRaw - baseMin) / (baseMax - baseMin);
    float modVal = (modRaw - baseMin) / (baseMax - baseMin);

    float startAngle = juce::MathConstants<float>::pi * 1.25f;
    float endAngle = juce::MathConstants<float>::pi * 2.75f;

    // Map to angles
    float baseAngle = startAngle + baseVal * (endAngle - startAngle);
    float modAngle = startAngle + modVal * (endAngle - startAngle);

    // Draw the overlay arc
    g.setColour(MinimalStyle::accentOrange);

    float minAngle = juce::jmin(baseAngle, modAngle);
    float delta = std::abs(modAngle - baseAngle);

    juce::Path modArc;
    modArc.addCentredArc(center.x, center.y, radius + 2.0f, radius + 2.0f,
        0.0f, minAngle, minAngle + delta, true);

    g.strokePath(modArc, juce::PathStrokeType(3.0f));

    juce::Slider::paint(g);
}

void ItnSlider::resized() {
    juce::Slider::resized();
    auto area = getLocalBounds().removeFromBottom(15);
}

void ItnSlider::setModMatrix(ModMatrix* matrix, const juce::String pName) {
    modMatrix = matrix;
    paramName = pName;
}

void ItnSlider::addModLinkSubmenu(juce::PopupMenu& menu) {
    if (!modMatrix || paramName == "") return;
    menu.addSeparator();

    juce::PopupMenu sub;

    std::vector<juce::String> sourceNames;
    modMatrix->getAllSourceNames(sourceNames);
    std::sort(sourceNames.begin(), sourceNames.end());
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
    sub.addItem("Set mod opacity...", [this, conn] {
        auto entry = std::make_unique<InlineValueEntry<double>>(conn->getOpacity());
        entry->linkToComponent<ItnSlider>(this, [conn](ItnSlider*, double val) {
            conn->setOpacity(val);
        });
        entry->setSize(50, 25);

        juce::CallOutBox::launchAsynchronously(
            std::move(entry),
            getScreenBounds(),
            nullptr
        );
    });
    sub.addItem("Set mod depth...", [this, conn] {
        auto entry = std::make_unique<InlineValueEntry<double>>(conn->getDepth());
        entry->linkToComponent<ItnSlider>(this, [conn](ItnSlider*, double val) {
            conn->setDepth(val);
        });
        entry->setSize(50, 25);

        juce::CallOutBox::launchAsynchronously(
            std::move(entry),
            getScreenBounds(),
            nullptr
        );
    });

    menu.addSubMenu(sourceName, sub);
}