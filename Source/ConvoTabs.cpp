/*
  ==============================================================================

    ConvoTabs.cpp
    Created: 9 Sep 2026 1:36:22pm
    Author:  BroDe

  ==============================================================================
*/

#include "ConvoTabs.h"


ConvoTabs::ConvoTabs() : tabbedComponent(juce::TabbedButtonBar::TabsAtTop) {
    setLookAndFeel(&ItnLookAndFeel::getInstance());

    viewports.add(new ConvoViewport());
}

ConvoTabs::~ConvoTabs() {
    setLookAndFeel(nullptr);
}

ConvoViewport* ConvoTabs::getViewport(int tabIndex) {
    int numTabs = tabbedComponent.getNumTabs();
    if (tabIndex < 0 || tabIndex >= numTabs) {
        DBG("ERROR: Tab index out of bounds for selecting viewport.");
        return nullptr;
    }
    return viewports[tabIndex];
}

void ConvoTabs::addMessage(
    int tabIndex, const juce::String& role,
    const juce::String& message, bool createRevealed
) {
    ConvoViewport* viewport = getViewport(tabIndex);
    viewport->addMessage(role, message, createRevealed);
}

void ConvoTabs::setBounds(int x, int y, int width, int height) {
    Component::setBounds(x, y, width, height);
    for (ConvoViewport* viewport : viewports) {
        viewport->setBounds(x, y, width, height);
    }
}
