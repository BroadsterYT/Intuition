/*
  ==============================================================================

    LFOEditor.cpp
    Created: 26 Sep 2025 12:36:43am
    Author:  Brody

  ==============================================================================
*/

#include "LFOEditor.h"


LFOEditor::LFOEditor(LFOShape& shape) : shape(shape) {
    startTimerHz(60);
}

LFOEditor::~LFOEditor() {
    stopTimer();
}

void LFOEditor::paint(juce::Graphics& g) {
    g.fillAll(juce::Colours::black);

    if (shape.getNumPoints() < 2) return;
    shape.sortPoints();

    g.setColour(juce::Colours::white);

    juce::Path path;
    auto& p0 = shape.getPoint(0);
    float x0 = p0.time * getWidth();
    float y0 = (1.0f - p0.value) * getHeight();
    path.startNewSubPath(x0, y0);

    for (int i = 1; i < shape.getNumPoints(); ++i) {
        auto& p1 = shape.getPoint(i - 1);
        auto& p2 = shape.getPoint(i);

        x0 = p1.time * getWidth();
        y0 = (1.0f - p1.value) * getHeight();

        float x2 = p2.time * getWidth();
        float y2 = (1.0f - p2.value) * getHeight();

        float cx = x0 + (x2 - x0) / 2.0f;
        float cy = y0 + (y2 - y0) / 2.0f - p1.curve * getHeight();

        path.quadraticTo(cx, cy, x2, y2);
    }

    g.strokePath(path, juce::PathStrokeType(2.0f));

    for (int i = 0; i < shape.getNumPoints(); ++i) {
        LFOPoint& p1 = shape.getPoint(i);
        float x1 = p1.time * getWidth();
        float y1 = (1.0f - p1.value) * getHeight();

        g.setColour(juce::Colours::cyan);
        g.drawEllipse(x1 - 15.0f / 2, y1 - 15.0f / 2, 15.0f, 15.0f, 2.0f);
    }
}

void LFOEditor::mouseDown(const juce::MouseEvent& e) {
    LFOPoint* p = getNearestPoint(e);

    if (e.getNumberOfClicks() == 2) {
        if (!p) {  // Adding new point
            float t = (float)e.position.x / getWidth();
            float v = 1.0f - e.position.y / getHeight();
            shape.addPoint(t, v, 0.0f);
        }
        else {  // Deleting point
            shape.removePoint(p);
        }
    }

    if (e.mods.isRightButtonDown() && p) {
        juce::PopupMenu menu;
        menu.addItem("Set Time...", [this, p] {
            auto entry = std::make_unique<InlineValueEntry<float>>(p->time);
            entry->linkToComponent<LFOPoint>(p, [this](LFOPoint* p, float val) {
                p->time = val;

                if (val < 0.0f) {
                    p->time = 0.0f;
                }
                if (val > 1.0f) {
                    p->time = 1.0f;
                }

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
        menu.addItem("Set Value...", [this, p] {
            auto entry = std::make_unique<InlineValueEntry<float>>(p->value);
            entry->linkToComponent<LFOPoint>(p, [this](LFOPoint* p, float val) {
                p->value = val;

                if (val < 0.0f) {
                    p->value = 0.0f;
                }
                if (val > 1.0f) {
                    p->value = 1.0f;
                }

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
        menu.addItem("Set Curve...", [this, p] {
            auto entry = std::make_unique<InlineValueEntry<float>>(p->curve);
            entry->linkToComponent<LFOPoint>(p, [this](LFOPoint* p, float val) {
                p->curve = val;

                if (val < -0.5f) {
                    p->curve = -0.5f;
                }
                if (val > 0.5f) {
                    p->curve = 0.5f;
                }

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
}

void LFOEditor::mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) {
    LFOPoint* p = getNearestPoint(e);
    if (!p) return;

    if (wheel.deltaY > 0.0f) {
        p->curve += 0.1f;
    }
    else if (wheel.deltaY < 0.0f) {
        p->curve -= 0.1f;
    }

    p->curve = juce::jlimit<float>(-0.5f, 0.5f, p->curve);
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

void LFOEditor::timerCallback() {
    repaint();
}