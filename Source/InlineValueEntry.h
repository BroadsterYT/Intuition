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
    InlineValueEntry(float currentValue);

    void setValueChangeCallback(std::function<void(float)> callback);

    /// <summary>
    /// Links a specific component to the value entry and assigns a function to change that component's value
    /// </summary>
    /// <typeparam name="ComponentType">The type of component being linked to</typeparam>
    /// <param name="component">Pointer to the specific component instance to link to</param>
    /// <param name="setter">The function to use to change the linked component's value</param>
    template<typename ComponentType>
    void linkToComponent(ComponentType* component, std::function<void(ComponentType*, float)> setter) {
        if (component) {
            setValueChangeCallback([component, setter](float value) {
                setter(component, value);
            });
        }
    }

    void resized() override;

private:
    juce::TextEditor editor;
    std::function<void(float)> valueChangeCallback;

    void setExitKeyLambdas();
};