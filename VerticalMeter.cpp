#include "VerticalMeter.h"

VerticalMeter::VerticalMeter() : currentValue(0.0f)
{
}

VerticalMeter::~VerticalMeter() {}

void VerticalMeter::setGainReduction(float dBValue)
{
    // Assume dBValue is positive: 0 = no reduction, 60 = full reduction.
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
    // Get our drawing bounds and reduce slightly for a neat inset.
    auto bounds = getLocalBounds().toFloat().reduced(2.0f);

    // Draw a subtle background gradient.
    juce::ColourGradient bgGradient(juce::Colours::darkgrey, bounds.getCentreX(), bounds.getY(),
        juce::Colours::black, bounds.getCentreX(), bounds.getBottom(), false);
    g.setGradientFill(bgGradient);
    g.fillRect(bounds);

    // Draw a rounded rectangle frame.
    float cornerRadius = 6.0f;
    juce::Path meterFrame;
    meterFrame.addRoundedRectangle(bounds, cornerRadius);
    g.setColour(juce::Colours::grey);
    g.strokePath(meterFrame, juce::PathStrokeType(1.5f));

    // Compute the fill height (fraction from 0 to 1).
    float fraction = juce::jlimit(0.0f, 1.0f, currentValue / 60.0f);
    float fillHeight = bounds.getHeight() * fraction;
    juce::Rectangle<float> fillRect(bounds.getX(), bounds.getBottom() - fillHeight,
        bounds.getWidth(), fillHeight);

    // Create a multi-stop gradient for a modern fill look:
    // It goes from green at the top to yellow in the middle to red at the bottom.
    juce::ColourGradient fillGradient(juce::Colours::green, fillRect.getCentreX(), fillRect.getY(),
        juce::Colours::red, fillRect.getCentreX(), fillRect.getBottom(), false);
    fillGradient.addColour(0.5, juce::Colours::yellow);
    g.setGradientFill(fillGradient);

    // Draw the fill using a rounded rectangle.
    juce::Path fillPath;
    fillPath.addRoundedRectangle(fillRect, cornerRadius);
    g.fillPath(fillPath);

    // Overlay the current gain reduction (in dB) as text.
    g.setColour(juce::Colours::white);
    g.setFont(12.0f);
    g.drawFittedText(juce::String(currentValue, 1) + " dB", bounds.toNearestInt(),
        juce::Justification::centred, 1);
}
