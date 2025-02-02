#pragma once

#include <JuceHeader.h>

class AnalogMeter : public juce::Component, private juce::Timer
{
public:
    AnalogMeter();
    ~AnalogMeter() override;

    void setGainReduction(double newGainReduction);

    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    double gainReduction = 0.0;

    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalogMeter)
};
