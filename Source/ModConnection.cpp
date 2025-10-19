/*
  ==============================================================================

    ModConnection.cpp
    Created: 15 Oct 2025 11:23:56pm
    Author:  BroDe

  ==============================================================================
*/

#include "ModConnection.h"


ModConnection::ModConnection(
    ModSource* source,
    ModDestination* dest
) : source(source),
    dest(dest) {

}

ModSource* ModConnection::getSource() const {
    return source;
}

ModDestination* ModConnection::getDestination() const {
    return dest;
}

bool ModConnection::getActive() const {
    return active;
}

void ModConnection::setActive(bool newActive) {
    active = newActive;
}

bool ModConnection::getBipolar() const {
    return bipolar;
}

void ModConnection::setBipolar(bool newBipolar) {
    bipolar = newBipolar;
}

float ModConnection::getOpacity() const {
    return opacity;
}

void ModConnection::setOpacity(float newOpacity) {
    opacity = newOpacity;
}

float ModConnection::getDepth() const {
    return depth;
}

void ModConnection::setDepth(float newDepth) {
    depth = newDepth;
}
