/*
  ==============================================================================

    ModConnection.h
    Created: 15 Oct 2025 11:23:56pm
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include "ModSource.h"
#include "ModDestination.h"


class ModConnection {
public:
    ModConnection(
        ModSource* source,
        ModDestination* dest
    );

    ModSource* getSource() const;
    ModDestination* getDestination() const;

    bool getActive() const;
    void setActive(bool newActive);
    bool getBipolar() const;
    void setBipolar(bool newBipolar);
    float getOpacity() const;
    void setOpacity(float newOpacity);
    float getDepth() const;
    void setDepth(float newDepth);

private:
    ModSource* source = nullptr;
    ModDestination* dest = nullptr;
    bool active = true;
    bool bipolar = false;
    float opacity = 1.0f;
    float depth = 1.0f;
};