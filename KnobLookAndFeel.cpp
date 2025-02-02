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
    auto angleRange = rotaryEndAngle - rotaryStartAngle;

    // The angle corresponding to the current slider value:
    float angle = rotaryStartAngle + sliderPosProportional * angleRange;

    // Draw the background arc.
    {
        juce::Path backgroundArc;
        backgroundArc.addCentredArc(centreX, centreY, radius, radius, 0.0f,
            rotaryStartAngle, rotaryEndAngle, true);
        g.setColour(juce::Colours::darkgrey);
        g.strokePath(backgroundArc, juce::PathStrokeType(3.0f));
    }

    // Draw the value arc.
    {
        juce::Path valueArc;
        valueArc.addCentredArc(centreX, centreY, radius, radius, 0.0f,
            rotaryStartAngle, angle, true);
        g.setColour(juce::Colours::red);
        g.strokePath(valueArc, juce::PathStrokeType(3.0f));
    }

    // Draw the pointer (knob indicator).
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

    // Draw a small centre circle.
    g.setColour(juce::Colours::black);
    g.fillEllipse(centreX - 3.0f, centreY - 3.0f, 6.0f, 6.0f);

    // Draw tick marks and numeric labels around the knob.
    drawTickMarks(g, centreX, centreY, radius, rotaryStartAngle, angleRange, slider);
}

void KnobLookAndFeel::drawTickMarks(juce::Graphics& g,
    float centreX, float centreY, float radius,
    float startAngle, float angleRange,
    juce::Slider& slider)
{
    // Define positions for 5 tick marks (0%, 25%, 50%, 75%, 100%)
    static constexpr float tickValues[5] = { 0.0f, 0.25f, 0.5f, 0.75f, 1.0f };

    for (auto tv : tickValues)
    {
        // Convert from 0..1 to angle.
        float thisAngle = startAngle + tv * angleRange;

        // Coordinates at the outer edge of the circle.
        float tx = centreX + radius * std::cos(thisAngle);
        float ty = centreY + radius * std::sin(thisAngle);

        // Draw a small tick line.
        float tickLen = 5.0f;
        float tickInnerX = centreX + (radius - tickLen) * std::cos(thisAngle);
        float tickInnerY = centreY + (radius - tickLen) * std::sin(thisAngle);

        g.setColour(juce::Colours::grey);
        g.drawLine(tx, ty, tickInnerX, tickInnerY, 1.0f);

        // Increase the label distance for better visibility.
        float labelRadius = radius + 20.0f;
        float lx = centreX + labelRadius * std::cos(thisAngle);
        float ly = centreY + labelRadius * std::sin(thisAngle);

        // Convert the tick value to a string (e.g. "0", "25", "50", etc).
        // Here, we compute the value from the slider’s range.
        juce::String labelText = juce::String((int)juce::roundToInt(slider.getRange().getStart()
            + tv * slider.getRange().getLength()));

        g.setColour(juce::Colours::white);
        g.setFont(12.0f);
        // Increase text area size so that all numbers are fully visible.
        juce::Rectangle<float> textArea(lx - 15, ly - 10, 30, 20);
        g.drawFittedText(labelText, textArea.toNearestInt(), juce::Justification::centred, 1);
    }
}
