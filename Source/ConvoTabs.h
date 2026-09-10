/*
  ==============================================================================

    ConvoTabs.h
    Created: 9 Sep 2026 1:36:22pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include "ConvoViewport.h"
#include "ItnLookAndFeel.h"
#include <JuceHeader.h>


class ConvoTabs : public juce::Component {
public:
    ConvoTabs();
    ~ConvoTabs();

    ConvoViewport* getViewport(int tabIndex);
    
    void addMessage(
        int tabIndex,
        const juce::String& role,
        const juce::String& message,
        bool createRevealed = false
    );

    void setBounds(int x, int y, int width, int height);

private:
    juce::TabbedComponent tabbedComponent;
    juce::OwnedArray<ConvoViewport> viewports;
};
