/*
  ==============================================================================

    LFOEditor.cpp
    Created: 26 Sep 2025 12:36:43am
    Author:  Brody

  ==============================================================================
*/

#include "LFOEditor.h"


LFOEditor::LFOEditor(LFOShape& shape, float* phase) : shape(shape), phase(phase) {
    setLookAndFeel(&lookAndFeel);
}

LFOEditor::~LFOEditor() {
    setLookAndFeel(nullptr);
    stopTimer();
}

void LFOEditor::paint(juce::Graphics& g) {
    g.fillAll(GlowStyle::roomDark);

    if (shape.getNumPoints() < 2) return;
    if (!phase) return;

    lookAndFeel.drawLFO(g, getBounds().toFloat(), shape, *phase);
    for (int i = 0; i < shape.getNumPoints(); ++i) {
        const LFOPoint& point = shape.getPoint(i);
        lookAndFeel.drawLFOPoint(g, getBounds().toFloat(), point);
    }
}

void LFOEditor::mouseDown(const juce::MouseEvent& e) {
    LFOPoint* p = getNearestPoint(e);

    if (e.getNumberOfClicks() == 1 && p) {
        currentDraggedPoint = p;
    }
    else if (e.getNumberOfClicks() == 2) {
        if (!p) {  // Adding new point
            float t = (float)e.position.x / getWidth();
            float v = 1.0f - e.position.y / getHeight();
            shape.addPoint(t, v, 0.0f);
            //currentDraggedPoint = getNearestPoint(e);
        }
        else {  // Deleting point
            shape.removePoint(p);
        }
    }

    if (e.mods.isRightButtonDown() && p) {
        juce::PopupMenu menu;
        menu.addItem("Set Time...", [this, p] {
            auto entry = std::make_unique<InlineValueEntry<float>>(p->getTime());
            entry->linkToComponent<LFOPoint>(p, [this](LFOPoint* p, float val) {
                p->setTime(val);
                repaint();
            });
            entry->setSize(50, 25);

            juce::CallOutBox::launchAsynchronously(
                std::move(entry),
            getScreenBounds(),
            nullptr
            );
        });
        menu.addItem("Set Value...", [this, p] {
            auto entry = std::make_unique<InlineValueEntry<float>>(p->getValue());
            entry->linkToComponent<LFOPoint>(p, [this](LFOPoint* p, float val) {
                p->setValue(val);
                shape.sortPoints();
            });
            entry->setSize(50, 25);

            juce::CallOutBox::launchAsynchronously(
                std::move(entry),
                getScreenBounds(),
                nullptr
            );
        });
        menu.addItem("Set Curve...", [this, p] {
            auto entry = std::make_unique<InlineValueEntry<float>>(p->getCurve());
            entry->linkToComponent<LFOPoint>(p, [this](LFOPoint* p, float val) {
                p->setCurve(val);

                shape.sortPoints();
                repaint();
            });
            entry->setSize(50, 25);

            juce::CallOutBox::launchAsynchronously(
                std::move(entry),
                getScreenBounds(),
                nullptr
            );
        });

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this).withMousePosition());
    }
}

void LFOEditor::mouseUp(const juce::MouseEvent& e) {
    currentDraggedPoint = nullptr;
    shape.sortPoints();
}

void LFOEditor::mouseDrag(const juce::MouseEvent& e) {
    auto* p = currentDraggedPoint;
    if (!p) return;

    p->setTime(e.position.x / getWidth());
    p->setValue(1.0f - e.position.y / getHeight());

    if (p == &shape.getPoint(0)) {
        p->setTime(0.0f);
    }
    else if (p == &shape.getPoint(shape.getNumPoints() - 1)) {
        p->setTime(1.0f);
    }
}

void LFOEditor::mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) {
    LFOPoint* p = getNearestPoint(e);
    if (!p) return;

    if (wheel.deltaY > 0.0f) {
        //p->curve += 0.1f;
        p->setCurve(p->getCurve() + 0.1f);
    }
    else if (wheel.deltaY < 0.0f) {
        //p->curve -= 0.1f;
        p->setCurve(p->getCurve() - 0.1f);
    }
}

LFOPoint* LFOEditor::getNearestPoint(const juce::MouseEvent& e) {
    LFOPoint* nearest = nullptr;
    float minDist = std::numeric_limits<float>::max();

    for (int i = 0; i < shape.getNumPoints(); ++i) {
        auto& pt = shape.getPoint(i);

        float px = pt.getTime() * getWidth();
        float py = (1.0f - pt.getValue()) * getHeight();

        float dx = e.position.x - px;
        float dy = e.position.y - py;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist < minDist) {
            minDist = dist;
            nearest = &pt;
        }
    }

    if (nearest && minDist < 10.0f) {
        return nearest;
    }

    return nullptr;
}

void LFOEditor::visibilityChanged() {
    if (isShowing()) {
        startTimerHz(60);
    }
    else {
        stopTimer();
    }
}

void LFOEditor::timerCallback() {
    repaint();
}