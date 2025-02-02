#include "VerticalMeter.h"

VerticalMeter::VerticalMeter()
    : currentValue(0.0f) // start at 0 dB reduction
{
}

VerticalMeter::~VerticalMeter() {}

void VerticalMeter::setGainReduction(float dBValue)
{
    // Now assume dBValue is positive (0 = no reduction, 60 = full reduction).
    dBValue = juce::jlimit(0.0f, 60.0f, dBValue);
    if (dBValue != currentValue)
    {
        currentValue = dBValue;
        repaint();
    }
}

void VerticalMeter::setGainReduction(double dBValue)
{
    setGainReduction(static_cast<float>(dBValue));
}

void VerticalMeter::paint(juce::Graphics& g)
{
    auto fullBounds = getLocalBounds().toFloat();
    g.fillAll(juce::Colours::black);

    // Map currentValue (0 to 60 dB) to a fraction (0 to 1).
    float fraction = juce::jlimit(0.0f, 1.0f, currentValue / 60.0f);
    float meterHeight = fullBounds.getHeight() * fraction;
    juce::Rectangle<float> fillArea = fullBounds.withY(fullBounds.getBottom() - meterHeight)
        .withHeight(meterHeight);

    // Use a gradient from green (low reduction) to red (high reduction)
    juce::ColourGradient cg(juce::Colours::green,
        fillArea.getCentreX(), fillArea.getY(),
        juce::Colours::red,
        fillArea.getCentreX(), fillArea.getBottom(),
        false);
    g.setGradientFill(cg);
    g.fillRect(fillArea);

    // Draw the outline.
    g.setColour(juce::Colours::grey);
    g.drawRect(fullBounds, 1.0f);

    // Optionally, draw the current gain reduction value.
    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawFittedText(juce::String(currentValue, 1) + " dB", fullBounds.toNearestInt(),
        juce::Justification::centred, 1);
}
