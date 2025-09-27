/*
  ==============================================================================

    InlineValueEntry.h
    Created: 25 Sep 2025 7:47:05pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


class InlineValueEntry : public juce::Component {
public:
    InlineValueEntry(double currentValue);

    void setValueChangeCallback(std::function<void(double)> callback);

    template<typename ComponentType>
    void linkToComponent(ComponentType* component, std::function<void(ComponentType*, double)> setter) {
        if (component) {
            setValueChangeCallback([component, setter](double value) {
                setter(component, value);
            });
        }
    }

    void resized() override;

private:
    juce::TextEditor editor;
    std::function<void(double)> valueChangeCallback;

    void setExitKeyLambdas();
};