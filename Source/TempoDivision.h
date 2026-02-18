/*
  ==============================================================================

    TempoDivision.h
    Created: 18 Feb 2026 10:04:59am
    Author:  BroDe

  ==============================================================================
*/

#pragma once

enum class TempoDivision {
    FourBars,
    TwoBars,
    Whole,
    Half,
    Quarter,
    Eighth,
    Sixteenth,
    ThirtySecond,
    SixtyFourth
};

inline float divisionToBeats(TempoDivision div) {
    switch (div) {
    case TempoDivision::FourBars:
        return 16.0f;
    case TempoDivision::TwoBars:
        return 8.0f;
    case TempoDivision::Whole:
        return 4.0f;
    case TempoDivision::Half:
        return 2.0f;
    case TempoDivision::Quarter:
        return 1.0f;
    case TempoDivision::Eighth:
        return 0.5f;
    case TempoDivision::Sixteenth:
        return 0.25f;
    case TempoDivision::ThirtySecond:
        return 0.125f;
    case TempoDivision::SixtyFourth:
        return 0.0625f;
    }

    return 1.0f;
}