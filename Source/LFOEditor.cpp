/*
  ==============================================================================

    LFOEditor.cpp
    Created: 26 Sep 2025 12:36:43am
    Author:  Brody

  ==============================================================================
*/

#include "LFOEditor.h"


LFOEditor::LFOEditor() {

}

void LFOEditor::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::lightcyan);
    if (shape.getNumPoints() < 2) return;
    shape.sortPoints();

    for (int i = 0; i < shape.getNumPoints() - 1; ++i) {
        auto& p1 = shape.getPoint(i);
        auto& p2 = shape.getPoint(i + 1);

        float x1 = p1.time * getWidth();
        float y1 = (1.0f - p1.value) * getHeight();

        auto x2 = p2.time * getWidth();
        auto y2 = (1.0f - p2.value) * getHeight();

        // TODO: Display bezier curve
        g.drawLine(x1, y1, x2, y2, 2.0f);
        g.drawEllipse(x1 - 15.0f / 2, y1 - 15.0f / 2, 15.0f, 15.0f, 2.0f);
    }
}

void LFOEditor::mouseDown(const juce::MouseEvent& e) {
    if (e.mods.isRightButtonDown()) {
        LFOPoint* p = getNearestPoint(e);
        if (!p) {  // Adding new point
            float t = (float)e.position.x / getWidth();
            float v = 1.0f - e.position.y / getHeight();
            shape.addPoint(t, v, 0.0f);
        }
        else {  // Popup menu
            juce::PopupMenu menu;
            menu.addItem("Set Time...", [this, p] {/*NYI*/});
            menu.addItem("Set Value...", [this, p] {/*NYI*/});
            menu.addItem("Set Curve...", [this, p] {/*NYI*/});

            menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this).withMousePosition());
        }
    }
    repaint();
}

void LFOEditor::mouseDrag(const juce::MouseEvent& e) {
    LFOPoint* p = getNearestPoint(e);
    if (!p) return;

    p->time = juce::jlimit(0.0f, 1.0f, e.position.x / getWidth());
    p->value = juce::jlimit(0.0f, 1.0f, 1.0f - e.position.y / getHeight());

    if (p == &shape.getPoint(0)) {
        p->time = 0.0f;
    }
    else if (p == &shape.getPoint(shape.getNumPoints() - 1)) {
        p->time = 1.0f;
    }

    repaint();
}

LFOPoint* LFOEditor::getNearestPoint(const juce::MouseEvent& e) {
    LFOPoint* nearest = nullptr;
    float minDist = std::numeric_limits<float>::max();

    for (int i = 0; i < shape.getNumPoints(); ++i) {
        auto& pt = shape.getPoint(i);

        float px = pt.time * getWidth();
        float py = (1.0f - pt.value) * getHeight();

        float dx = e.position.x - px;
        float dy = e.position.y - py;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist < minDist) {
            minDist = dist;
            nearest = &pt;
        }
    }

    if (nearest && minDist < 15.0f) {
        return nearest;
    }

    return nullptr;
}