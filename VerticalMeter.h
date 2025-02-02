#pragma once

#include <JuceHeader.h>

class VerticalMeter : public juce::Component
{
public:
    VerticalMeter();
    ~VerticalMeter() override;

    void setGainReduction(float dBValue);
    void setGainReduction(double dBValue); // Overloaded method

    void paint(juce::Graphics& g) override;

private:
    float currentValue = -60.0f;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VerticalMeter)
};
