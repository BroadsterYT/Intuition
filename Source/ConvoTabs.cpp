/*
  ==============================================================================

    ConvoTabs.cpp
    Created: 9 Sep 2026 1:36:22pm
    Author:  BroDe

  ==============================================================================
*/

#include "ConvoTabs.h"
#include "ItnFileHelper.h"


ConvoTabs::ConvoTabs() : tabbedComp(juce::TabbedButtonBar::TabsAtTop) {
    setLookAndFeel(&ItnLookAndFeel::getInstance());
    createTabsForConvoFiles();
    addAndMakeVisible(tabbedComp);
}

ConvoTabs::~ConvoTabs() {
    setLookAndFeel(nullptr);
}

ConvoViewport* ConvoTabs::getViewport(int tabIndex) {
    int numTabs = tabbedComp.getNumTabs();
    if (tabIndex < 0 || tabIndex >= numTabs) {
        DBG("ERROR: Tab index out of bounds for selecting viewport.");
        return nullptr;
    }
    return viewports[tabIndex];
}

ConvoViewport* ConvoTabs::getCurrentViewport() {
    int tabIndex = tabbedComp.getCurrentTabIndex();
    return viewports[tabIndex];
}

void ConvoTabs::addViewport(const juce::String& convoId) {
    auto* viewport = viewports.add(new ConvoViewport(convoId));
    tabbedComp.addTab("Test", juce::Colours::transparentBlack, viewport, false);
}

void ConvoTabs::addMessage(
    const juce::String& role,
    const juce::String& message, bool createRevealed
) {
    ConvoViewport* viewport = getCurrentViewport();
    viewport->addMessage(role, message, createRevealed);
}

void ConvoTabs::resized() {
    tabbedComp.setBounds(getLocalBounds());
}

void ConvoTabs::createTabsForConvoFiles() {
    juce::File convoDir = ItnFileHelper::getIntumiConvoFileDirectory();
    juce::Array<juce::File> convoFiles = convoDir.findChildFiles(juce::File::findFiles, false, "*");

    for (auto& file : convoFiles) {
        juce::String convoId = file.getFileNameWithoutExtension();
        addViewport(convoId);
    }
}
