#include "AnalogMeter.h"

AnalogMeter::AnalogMeter()
{
    startTimerHz(30); // Update 30 times per second
}

AnalogMeter::~AnalogMeter()
{
}

void AnalogMeter::setGainReduction(double newGainReduction)
{
    gainReduction = newGainReduction;
    repaint();
}

void AnalogMeter::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    // Draw the meter background
    g.setColour(juce::Colours::darkgrey);
    g.fillEllipse(getLocalBounds().toFloat());

    // Draw the needle
    g.setColour(juce::Colours::red);
    float angle = juce::jmap((float)gainReduction, 0.0f, 30.0f, -juce::MathConstants<float>::pi / 4, juce::MathConstants<float>::pi / 4);
    juce::Path needle;
    needle.addLineSegment(juce::Line<float>(getWidth() / 2.0f, getHeight() / 2.0f, getWidth() / 2.0f + std::cos(angle) * getWidth() / 2.0f, getHeight() / 2.0f - std::sin(angle) * getHeight() / 2.0f), 2.0f);
    g.strokePath(needle, juce::PathStrokeType(2.0f));
}

void AnalogMeter::resized()
{
}

void AnalogMeter::timerCallback()
{
    repaint();
}
