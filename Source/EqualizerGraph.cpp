/*
  ==============================================================================

    EqualizerGraph.cpp
    Created: 21 Feb 2026 1:23:40pm
    Author:  BroDe

  ==============================================================================
*/

#include "EqualizerGraph.h"
#include "BiquadResponse.h"

EqualizerGraph::EqualizerGraph(juce::AudioProcessorValueTreeState& vts, ModMatrix* modMatrix, EqualizerModule& equalizer
) : parameters(vts), modMatrix(modMatrix), equalizer(equalizer) {
    setLookAndFeel(&ItnLookAndFeel::getInstance());
    startTimerHz(30);
}

EqualizerGraph::~EqualizerGraph() {
    setLookAndFeel(nullptr);
    stopTimer();
}

void EqualizerGraph::mouseDown(const juce::MouseEvent& e) {
    auto band = getNearestEQBand(e);
    currentDraggedBand = band;
}

void EqualizerGraph::mouseUp(const juce::MouseEvent& /*e*/) {
    currentDraggedBand = nullptr;
}

void EqualizerGraph::mouseDrag(const juce::MouseEvent& e) {
    auto band = currentDraggedBand;
    if (!band) return;

    juce::RangedAudioParameter* freqParam = parameters.getParameter("EQBAND1_FREQUENCY");
    juce::RangedAudioParameter* gainParam = parameters.getParameter("EQBAND1_GAIN");

    // Getting new unnormed frequency
    float newFreq = BiquadResponse::getLinearValueAsFreq(e.position.x, (float)getWidth());
    auto& freqRange = freqParam->getNormalisableRange();
    newFreq = std::clamp(newFreq, freqRange.start, freqRange.end);
    
    // Getting new unnormed gain
    float yPos = 1.0f - e.position.y / (float)getHeight();
    float newGain = juce::jmap(yPos, -18.0f, 18.0f);
    auto& gainRange = gainParam->getNormalisableRange();
    newGain = std::clamp(newGain, -18.0f, 18.0f);
    
    int bandIndex = band->getId();
    switch (bandIndex) {
    case 0:
        break;
    case 1:
        freqParam = parameters.getParameter("EQBAND2_FREQUENCY");
        gainParam = parameters.getParameter("EQBAND2_GAIN");
        break;
    case 2:
        freqParam = parameters.getParameter("EQBAND3_FREQUENCY");
        gainParam = parameters.getParameter("EQBAND3_GAIN");
        break;
    case 3:
        freqParam = parameters.getParameter("EQBAND4_FREQUENCY");
        gainParam = parameters.getParameter("EQBAND4_GAIN");
        break;
    case 4:
        freqParam = parameters.getParameter("EQBAND5_FREQUENCY");
        gainParam = parameters.getParameter("EQBAND5_GAIN");
        break;
    case 5:
        freqParam = parameters.getParameter("EQBAND6_FREQUENCY");
        gainParam = parameters.getParameter("EQBAND6_GAIN");
        break;
    case 6:
        freqParam = parameters.getParameter("EQBAND7_FREQUENCY");
        gainParam = parameters.getParameter("EQBAND7_GAIN");
        break;
    case 7:
        freqParam = parameters.getParameter("EQBAND8_FREQUENCY");
        gainParam = parameters.getParameter("EQBAND8_GAIN");
        break;
    }

    float normFreq = freqRange.convertTo0to1(newFreq);
    float normGain = gainRange.convertTo0to1(newGain);

    freqParam->setValueNotifyingHost(normFreq);
    gainParam->setValueNotifyingHost(normGain);
}

void EqualizerGraph::mouseWheelMove(const juce::MouseEvent& e, const juce::MouseWheelDetails& wheel) {
    const EQBand* band = getNearestEQBand(e);
    if (!band) return;
    
    juce::RangedAudioParameter* qParam = parameters.getParameter("EQBAND1_Q");
    int bandIndex = band->getId();
    switch (bandIndex) {
    case 0:
        break;
    case 1:
        qParam = parameters.getParameter("EQBAND2_Q");
        break;
    case 2:
        qParam = parameters.getParameter("EQBAND3_Q");
        break;
    case 3:
        qParam = parameters.getParameter("EQBAND4_Q");
        break;
    case 4:
        qParam = parameters.getParameter("EQBAND5_Q");
        break;
    case 5:
        qParam = parameters.getParameter("EQBAND6_Q");
        break;
    case 6:
        qParam = parameters.getParameter("EQBAND7_Q");
        break;
    case 7:
        qParam = parameters.getParameter("EQBAND8_Q");
        break;
    }

    // Unnormalizing
    auto& qRange = qParam->getNormalisableRange();
    float newQ = qRange.convertFrom0to1(qParam->getValue());
    if (wheel.deltaY > 0.0f) {  // Scroll up
        newQ += 0.2f;
    }
    else if (wheel.deltaY < 0.0f) {  // Scroll down
        newQ -= 0.2f;
    }
    // Re-normalizing
    newQ = std::clamp(newQ, qRange.start, qRange.end);
    float normQ = qRange.convertTo0to1(newQ);
    qParam->setValueNotifyingHost(normQ);
}

const EQBand* EqualizerGraph::getNearestEQBand(const juce::MouseEvent& e) {
    const EQBand* nearest = nullptr;
    float minDist = std::numeric_limits<float>::max();

    for (int i = 0; i < 8; ++i) {
        auto band = &equalizer.getBand(i);

        // Converting band parameters to canvas space
        auto bounds = getLocalBounds().toFloat();
        float px = BiquadResponse::getFreqInLinearRange(band->getFrequency(), bounds.getWidth());
        float py = juce::jmap(band->getGain(), -18.0f, 18.0f, bounds.getHeight(), 0.0f);

        float dx = e.position.x - px;
        float dy = e.position.y - py;
        float dist = std::sqrt(dx * dx + dy * dy);
        if (dist < minDist) {
            minDist = dist;
            nearest = band;
        }
    }

    if (nearest && minDist < 10.0f) {
        DBG("Nearest band with freq " << nearest->getFrequency() << ".");
        return nearest;
    }
    DBG("No bands near...");
    return nullptr;
}

void EqualizerGraph::paint(juce::Graphics& g) {
    std::vector<std::vector<float>> bandCoeffs;
    for (int i = 0; i < 8; ++i) {
        auto& band = equalizer.getBand(i);
        std::vector<float> coeffs;
        band.getBiquadCoefficients(coeffs);
        bandCoeffs.push_back(coeffs);
    }
    jassert(bandCoeffs.size() > 0);
    ItnLookAndFeel::drawEqualizer(g, getLocalBounds().toFloat(), bandCoeffs);

    // Drawing band locations
    for (int i = 0; i < 8; ++i) {
        ItnLookAndFeel::drawEqualizerPoint(g, getLocalBounds().toFloat(), equalizer.getBand(i));
    }
}

void EqualizerGraph::timerCallback() {
    repaint();
}
