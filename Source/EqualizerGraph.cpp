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
        menu.addItem("Active", true, band->isEnabled(), [this, band] {
            if (band->isEnabled()) updateBandParameter(band->getId(), "ENABLED", false);
            else updateBandParameter(band->getId(), "ENABLED", true);
        });
        menu.addItem("Set Frequency...",
            [this, band, callOutW, callOutH] {
                auto entry = std::make_unique<InlineValueEntry>(band->getFrequency());
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
                auto entry = std::make_unique<InlineValueEntry>(band->getGain());
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
                auto entry = std::make_unique<InlineValueEntry>(band->getQuality());
                entry->linkToComponent<const EQBand>(band,
                    [this](const EQBand* b, float newQ) {
                        updateBandParameter(b->getId(), "Q", newQ);
                    }
                );
                entry->setSize(callOutW, callOutH);
                juce::CallOutBox::launchAsynchronously(std::move(entry), getScreenBounds(), nullptr);
            }
        );
        addFilterSelectionSubmenu(band, menu);
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
    // Drawing Gridlines
    g.fillAll(MinimalStyle::bgDarkest);
    drawGridlines(g);
    
    bool fftCheck1 = equalizer.getFFTData(fftBuffer1, 0);
    bool fftCheck2 = equalizer.getFFTData(fftBuffer2, 1);
    if (fftCheck1 && fftCheck2) {
        fftSumBuffer.resize(fftBuffer1.size());
        for (int i = 0; i < fftBuffer1.size(); ++i) {
            fftSumBuffer[i] = fftBuffer1[i];
            fftSumBuffer[i] += fftBuffer2[i];
        }
    }
    ItnLookAndFeel::drawEqualizerOutput(g, getLocalBounds().toFloat(), fftSumBuffer);

    // Drawing frequency curve
    std::vector<std::vector<float>> bandCoeffs;
    for (int i = 0; i < 8; ++i) {
        auto& band = equalizer.getBand(i);
        if (!band.isEnabled()) continue;

        std::vector<float> coeffs;
        band.getBiquadCoefficients(coeffs);
        bandCoeffs.push_back(coeffs);
    }
    ItnLookAndFeel::drawEqualizerCurve(g, getLocalBounds().toFloat(), bandCoeffs);

    // Drawing band locations
    for (int i = 0; i < 8; ++i) {
        ItnLookAndFeel::drawEqualizerPoint(g, getLocalBounds().toFloat(), equalizer.getBand(i));
    }
}

void EqualizerGraph::drawGridlines(juce::Graphics& g) {
    juce::Path path;

    // Additional log lines
    auto drawEquidistantLines = [&](int startFreq, int endFreq, int inc) {
        for (int i = startFreq; i < endFreq; i += inc) {
            float normFreq = BiquadResponse::getFreqInLinearRange((float)i, (float)getWidth());
            path.startNewSubPath(normFreq, 0.0f);
            path.lineTo(normFreq, (float)getHeight());
        }
    };

    drawEquidistantLines(25, 100, 5);
    drawEquidistantLines(100, 1000, 50);
    drawEquidistantLines(1000, 20000, 1000);

    for (int i = -12; i < 18; i = i + 6) {
        float realGain = (float)juce::jmap(i, -18, 18, 0, getHeight());
        path.startNewSubPath(0.0f, realGain);
        path.lineTo((float)getWidth(), realGain);
    }

    g.setColour(MinimalStyle::textSecondary.withAlpha(0.25f));
    g.strokePath(path, juce::PathStrokeType(1.0f));
}

void EqualizerGraph::addFilterSelectionSubmenu(const EQBand* band, juce::PopupMenu& menu) {
    juce::PopupMenu sub;

    auto type = band->getFilterType();
    auto checkType = [type](const FilterType comp) {
        if (type == comp) return true;
        return false;
    };
    
    //sub.addSectionHeader("High-Passes");
    addFilterSelectionItem(band, FilterType::HighPass, "High-Pass", sub);
    addFilterSelectionItem(band, FilterType::HighShelf, "High Shelf", sub);

    //sub.addSectionHeader("Peaks");
    addFilterSelectionItem(band, FilterType::Peaking, "Peaking", sub);
    addFilterSelectionItem(band, FilterType::Notch, "Notch", sub);

    //sub.addSectionHeader("Low-Passes");
    addFilterSelectionItem(band, FilterType::LowPass, "Low-Pass", sub);
    addFilterSelectionItem(band, FilterType::LowShelf, "Low Shelf", sub);
    
    menu.addSubMenu("Set Filter Type...", sub);
}

void EqualizerGraph::addFilterSelectionItem(const EQBand* band, const FilterType& itemType, const juce::String& displayName, juce::PopupMenu& menu) {
    bool markActiveSelection = false;
    auto type = band->getFilterType();
    if (type == itemType) markActiveSelection = true;

    menu.addItem(displayName, true, markActiveSelection,
        [this, band, itemType] {
            updateBandParameter(band->getId(), "FILTER_TYPE", (int)itemType);
        }
    );
}

void EqualizerGraph::timerCallback() {
    repaint();
}
