/*
  ==============================================================================

    LFOShape.cpp
    Created: 26 Sep 2025 12:56:40am
    Author:  BroDe

  ==============================================================================
*/

#include "LFOShape.h"


LFOShape::LFOShape() {
    points.push_back(LFOPoint{ 0.0f, 0.0f, 0.0f });
    points.push_back(LFOPoint{ 0.5f, 1.0f, 0.0f });
    points.push_back(LFOPoint{ 1.0f, 0.0f, 0.0f });
}

LFOPoint& LFOShape::getPoint(int index) {
    if (index < 0) {
        return points[0];
    }
    if (index >= points.size()) {
        return points[points.size() - 1];
    }

    return points[index];
}

void LFOShape::addPoint(float time, float value, float curve) {
    sortPoints();

    int idxToInsert = 0;
    for (int i = 0; i < points.size(); ++i) {
        if (points[i].time >= time) {
            idxToInsert = i;
            break;
        }
    }

    LFOPoint newPoint = { time, value, curve };
    points.insert(points.begin() + idxToInsert, newPoint);
}

void LFOShape::removePoint(LFOPoint* point) {
    if (!point) return;
    for (int i = 0; i < points.size(); ++i) {
        if (point == &points[i]) {
            points.erase(points.begin() + i);
            return;
        }
    }
}

int LFOShape::getNumPoints() {
    return points.size();
}

void LFOShape::sortPoints() {
    std::sort(points.begin(), points.end(), [](const LFOPoint& a, const LFOPoint& b) {
        return a.time < b.time;
        });
}

float LFOShape::getValue(float phase) {
    // Can guarantee at least 3 points in LFO
    if (points.size() < 3) {
        return 0.0f;
    }


}

float LFOShape::interpolateBezier(const LFOPoint& p0, const LFOPoint& p2, float phase, float curveAmount) {
    float localT = (phase - p0.time) / (p2.time - p0.time);
    float yMid = 0.5f * (p0.value + p2.value) + curveAmount;

    float B = (1 - localT) * (1 - localT) * p0.value
              + 2.0f * (1 - localT) * localT * yMid
              + localT * localT * p2.value ;

    return B;
}