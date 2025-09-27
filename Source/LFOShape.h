/*
  ==============================================================================

    LFOShape.h
    Created: 26 Sep 2025 12:56:34am
    Author:  BroDe

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


struct LFOPoint {
    float time = 0.0f;  // Normalized 0 - 1 in LFO cycle
    float value = 0.0f;  // Normalized 0 - 1 for LFO amplitude
    float curve = 1.0f;

    bool operator==(const LFOPoint& other) {
        bool output = true;

        if (time != other.time) {
            output = false;
        }
        if (value != other.value) {
            output = false;
        }

        return output;
    }
};


class LFOShape {
public:
    LFOShape();

    LFOPoint& getPoint(int index);
    void addPoint(float time, float value, float curve);
    int getNumPoints();
    void sortPoints();
    
    float getValue(float phase);
    float interpolateBezier(const LFOPoint& p0, const LFOPoint& p2, float phase, float curveAmount);

private:
    std::vector<LFOPoint> points;
    bool loop = true;
};
