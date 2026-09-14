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
#include "ItnTabbedComponent.h"
#include <JuceHeader.h>


class ConvoTabs : public juce::Component {
public:
    ConvoTabs();
    ~ConvoTabs();

    ConvoViewport* getViewport(int tabIndex);
    ConvoViewport* getCurrentViewport();
    void addViewport(const juce::String& convoId);
    
    void addMessage(
        const juce::String& role,
        const juce::String& message,
        bool createRevealed = false
    );

    void resized() override;

private:
    ItnTabbedComponent tabbedComp;
    juce::OwnedArray<ConvoViewport> viewports;

    /// <summary>
    /// Creates a tab for each existing Intumi conversation. (Each individual
    /// log file in the Intumi logs directory constitutes a unique conversation)
    /// </summary>
    void createTabsForConvoFiles();
};
