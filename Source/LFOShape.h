/*
  ==============================================================================

    LFOShape.h
    Created: 26 Sep 2025 12:56:34am
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "LFOPoint.h"


class LFOShape {
public:
    LFOShape();

    LFOPoint& getPoint(int index);
    void addPoint(float time, float value, float curve);
    void removePoint(LFOPoint* point);
    int getNumPoints();
    void sortPoints();
    
    float getValue(float phase);
    float interpolateBezier(const LFOPoint& p0, const LFOPoint& p2, float phase, float curveAmount);

private:
    std::vector<LFOPoint> points;
    bool loop = true;
};
