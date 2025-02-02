#pragma once

#include <JuceHeader.h>

// A simple ring-style rotary knob
class KnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    KnobLookAndFeel() {}
    ~KnobLookAndFeel() override {}

    void drawRotarySlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider& slider) override;

    // Utility to draw ticks for e.g. 0%, 25%, 50%, 75%, 100%
    void drawTickMarks(juce::Graphics& g,
        float centreX, float centreY, float radius,
        float startAngle, float angleRange,
        juce::Slider& slider);
};
