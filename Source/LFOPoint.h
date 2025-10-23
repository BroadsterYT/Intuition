/*
  ==============================================================================

    LFOPoint.h
    Created: 1 Oct 2025 1:17:37am
    Author:  BroDe

  ==============================================================================
*/

#pragma once


class LFOPoint {
public:
    LFOPoint(float time = 0.0f, float value = 0.0f, float curve = 0.0f);

    void setTime(float newTime);
    float getTime() const;

    void setValue(float newVal);
    float getValue() const;

    void setCurve(float newCurve);
    float getCurve() const;
    void setCurveBounds(float newMin, float newMax);

private:
    float time = 0.0f;
    float value = 0.0f;
    float curve = 0.0f;

    float curveMin = -0.5f;
    float curveMax = 0.5f;
};