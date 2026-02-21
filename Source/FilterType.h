/*
  ==============================================================================

    FilterType.h
    Created: 20 Feb 2026 10:04:00pm
    Author:  BroDe

    Contains the filterType enum class that stores all possible types of filters
    that can be selected

  ==============================================================================
*/

#pragma once


enum class FilterType {
    HighPass,
    HighShelf,
    Peak,
    LowShelf,
    LowPass
};