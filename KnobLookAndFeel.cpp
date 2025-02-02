#include "KnobLookAndFeel.h"

void KnobLookAndFeel::drawRotarySlider(juce::Graphics& g,
    int x, int y, int width, int height,
    float sliderPosProportional,
    float rotaryStartAngle,
    float rotaryEndAngle,
    juce::Slider& slider)
{
    auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angleRange = rotaryEndAngle - rotaryStartAngle;

    // The angle corresponding to the current slider value:
    float angle = rotaryStartAngle + sliderPosProportional * angleRange;

    // draw background arc
    {
        juce::Path backgroundArc;
        backgroundArc.addCentredArc(centreX, centreY, radius, radius, 0.0f,
            rotaryStartAngle, rotaryEndAngle, true);
        g.setColour(juce::Colours::darkgrey);
        g.strokePath(backgroundArc, juce::PathStrokeType(3.0f));
    }

    // draw value arc
    {
        juce::Path valueArc;
        valueArc.addCentredArc(centreX, centreY, radius, radius, 0.0f,
            rotaryStartAngle, angle, true);
        g.setColour(juce::Colours::red);
        g.strokePath(valueArc, juce::PathStrokeType(3.0f));
    }

    // draw pointer (knob indicator)
    {
        float pointerLength = radius * 0.7f;
        float pointerThickness = 4.0f;
        juce::Path p;
        p.addRectangle(-pointerThickness * 0.5f, -radius,
            pointerThickness, pointerLength);
        p.applyTransform(juce::AffineTransform::rotation(angle)
            .translated(centreX, centreY));
        g.setColour(juce::Colours::green);
        g.fillPath(p);
    }

    // (Optional) small circle in the centre
    g.setColour(juce::Colours::black);
    g.fillEllipse(centreX - 3.0f, centreY - 3.0f, 6.0f, 6.0f);
}
