/*
  ==============================================================================

    ItnSlider.cpp
    Created: 25 Sep 2025 6:48:55pm
    Author:  Brody

  ==============================================================================
*/

#include "ItnSlider.h"


ItnSlider::ItnSlider() : tooltipController(*this) {
    startTimerHz(60);
    setLookAndFeel(&ItnLookAndFeel::getInstance());
    setSliderStyle(juce::Slider::Rotary);
    setTextBoxStyle(juce::Slider::NoTextBox, false, 40, 15);
    tooltipController.setTooltipText("DEFAULT");
}

ItnSlider::~ItnSlider() {
    setLookAndFeel(nullptr);
    stopTimer();
}

//void ItnSlider::parentHierarchyChanged() {
//    // Allows tooltip to be seen outside slider bounds (necessary!)
//    auto top = getTopLevelComponent();
//    top->addChildComponent(tooltip);
//}

void ItnSlider::setLabelName(const juce::String newName) {
    /*name = newName;
    updateLabel();*/
}

void ItnSlider::setCustomTooltipText(const juce::String parameterKey) {
    /*auto jsonText = juce::String::fromUTF8(BinaryData::tooltips_json, BinaryData::tooltips_jsonSize);
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
    tooltip.setDescription(description, true);*/
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

//void ItnSlider::mouseEnter(const juce::MouseEvent& e) {
//    hovering = true;
//}
//
//void ItnSlider::mouseExit(const juce::MouseEvent& e) {
//    hovering = false;
//    tooltipSpawnTimer = 0;
//    tooltipVisible = false;
//    tooltip.hide();
//}
//
//void ItnSlider::mouseDrag(const juce::MouseEvent& e) {
//    if (tooltipVisible) {
//        tooltipVisible = false;
//        tooltip.hide();
//    }
//
//    tooltipSpawnTimer = 0;
//    hovering = false;
//
//    juce::Slider::mouseDrag(e);
//}

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

    //label.setBounds(area);
}

void ItnSlider::setModMatrix(ModMatrix* matrix, const juce::String pName) {
    modMatrix = matrix;
    paramName = pName;
}

void ItnSlider::updateLabel() {
    //label.setText(name, juce::dontSendNotification);
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

void ItnSlider::timerCallback() {
    repaint();

    //if (hovering) {
    //    tooltipSpawnTimer += 1;
    //}

    //if (tooltipSpawnTimer >= 30 && !tooltipVisible) {
    //    tooltipVisible = true;
    //    
    //    // Placing tooltip within window bounds
    //    juce::Point<int> tooltipPos(0, 0);
    //    auto* top = getTopLevelComponent();
    //    auto posInWindow = top->getLocalPoint(this, juce::Point<int>(0, 0));

    //    if (posInWindow.getX() >= top->getWidth() / 2) {
    //        tooltipPos.setX(posInWindow.getX() - tooltip.getFullTextWidth());
    //    }
    //    else {
    //        tooltipPos.setX(posInWindow.getX() + getWidth());
    //    }
    //    if (posInWindow.getY() >= top->getHeight() / 2) {
    //        tooltipPos.setY(posInWindow.getY() - tooltip.getFullTextHeight());
    //    }
    //    else {
    //        tooltipPos.setY(posInWindow.getY() + getHeight());
    //    }

    //    tooltip.show(
    //        tooltipPos.getX(),
    //        tooltipPos.getY()
    //    );
    //}
}