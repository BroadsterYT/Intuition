/*
  ==============================================================================

    LFOPoint.cpp
    Created: 1 Oct 2025 1:17:45am
    Author:  BroDe

  ==============================================================================
*/

#include "LFOPoint.h"


LFOPoint::LFOPoint(float time = 0.0f, float value = 0.0f, float curve = 0.0f) {
    setTime(time);
    setValue(value);
    setCurve(curve);
}

void LFOPoint::setTime(float newTime) {
    time = newTime;
    if (time < 0.0f) time = 0.0f;
    else if (time > 1.0f) time = 1.0f;
}

float LFOPoint::getTime() const {
    return time;
}

void LFOPoint::setValue(float newVal) {
    value = newVal;
    if (value < 0.0f) value = 0.0f;
    else if (value > 1.0f) value = 1.0f;
}

float LFOPoint::getValue() const {
    return value;
}

void LFOPoint::setCurve(float newCurve) {
    curve = newCurve;
    if (curve < curveMin) curve = curveMin;
    else if (curve > curveMax) curve = curveMax;
}

float LFOPoint::getCurve() const {
    return curve;
}

void LFOPoint::setCurveBounds(float newMin, float newMax) {
    curveMin = newMin;
    curveMax = newMax;
    setCurve(curve);
}
