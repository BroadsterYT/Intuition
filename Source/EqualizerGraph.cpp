/*
  ==============================================================================

    EqualizerGraph.cpp
    Created: 21 Feb 2026 1:23:40pm
    Author:  BroDe

  ==============================================================================
*/

#include "EqualizerGraph.h"
#include "BiquadResponse.h"
#include "InlineValueEntry.h"


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

    if (e.mods.isRightButtonDown() && band) {
        const int callOutW = 75;  // CallOut width
        const int callOutH = 25;  // CallOut height

        juce::PopupMenu menu;
        menu.addItem("Set Frequency...",
            [this, band, callOutW, callOutH] {
                auto entry = std::make_unique<InlineValueEntry<float>>(band->getFrequency());
                entry->linkToComponent<const EQBand>(band,
                    [this](const EQBand* b, float newFreq) {
                        updateBandParameter(b->getId(), "FREQUENCY", newFreq);
                    }
                );
                entry->setSize(callOutW, callOutH);
                juce::CallOutBox::launchAsynchronously(std::move(entry), getScreenBounds(), nullptr);
            }
        );
        menu.addItem("Set Gain...",
            [this, band, callOutW, callOutH] {
                auto entry = std::make_unique<InlineValueEntry<float>>(band->getGain());
                entry->linkToComponent<const EQBand>(band,
                    [this](const EQBand* b, float newGain) {
                        updateBandParameter(b->getId(), "GAIN", newGain);
                    }
                );
                entry->setSize(callOutW, callOutH);
                juce::CallOutBox::launchAsynchronously(std::move(entry), getScreenBounds(), nullptr);
            }
        );
        menu.addItem("Set Q...",
            [this, band, callOutW, callOutH] {
                auto entry = std::make_unique<InlineValueEntry<float>>(band->getQuality());
                entry->linkToComponent<const EQBand>(band,
                    [this](const EQBand* b, float newQ) {
                        updateBandParameter(b->getId(), "Q", newQ);
                    }
                );
                entry->setSize(callOutW, callOutH);
                juce::CallOutBox::launchAsynchronously(std::move(entry), getScreenBounds(), nullptr);
            }
        );
        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this).withMousePosition());
    }
}

void EqualizerGraph::mouseUp(const juce::MouseEvent& /*e*/) {
    currentDraggedBand = nullptr;
}

void EqualizerGraph::mouseDrag(const juce::MouseEvent& e) {
    auto band = currentDraggedBand;
    if (!band) return;
    const int bandIndex = band->getId();

    auto freqParam = getBandParameter(bandIndex, juce::String("FREQUENCY"));
    auto gainParam = getBandParameter(bandIndex, juce::String("GAIN"));

    // Normalizing frequency parameter
    float newFreq = BiquadResponse::getLinearValueAsFreq(e.position.x, (float)getWidth());
    auto& freqRange = freqParam->getNormalisableRange();
    newFreq = std::clamp(newFreq, freqRange.start, freqRange.end);
    float normFreq = freqRange.convertTo0to1(newFreq);
    
    float normGain = 1.0f - e.position.y / (float)getHeight();

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
        return nearest;
    }
    return nullptr;
}

juce::RangedAudioParameter* EqualizerGraph::getBandParameter(const int index, const juce::String& paramSuffix) {
    jassert(index >= 0 && index <= 7);
    juce::String paramName;
    paramName << "EQBAND" << index + 1 << "_" << paramSuffix;
    return parameters.getParameter(paramName);
}

void EqualizerGraph::updateBandParameter(const int index, const juce::String& paramSuffix, float newValue) {
    jassert(index >= 0 && index <= 7);
    auto bandParam = getBandParameter(index, paramSuffix);
    auto paramRange = bandParam->getNormalisableRange();

    newValue = std::clamp(newValue, paramRange.start, paramRange.end);
    float normValue = bandParam->convertTo0to1(newValue);
    bandParam->setValueNotifyingHost(normValue);
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
