#pragma once

#include <JuceHeader.h>

class KnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawRotarySlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider& slider) override;

private:
    void drawTickMarks(juce::Graphics& g,
        float centreX, float centreY, float radius,
        float startAngle, float angleRange,
        juce::Slider& slider);
};
