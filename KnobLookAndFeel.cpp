#include "KnobLookAndFeel.h"
#include <cmath>

void KnobLookAndFeel::drawRotarySlider(juce::Graphics& g,
    int x, int y, int width, int height,
    float sliderPosProportional,
    float rotaryStartAngle,
    float rotaryEndAngle,
    juce::Slider& slider)
{
    // Establish bounds and centre.
    auto bounds = juce::Rectangle<float>(x, y, (float)width, (float)height);
    auto centre = bounds.getCentre();

    // Determine base radius, leaving some padding.
    const float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 10.0f;
    const float angleRange = rotaryEndAngle - rotaryStartAngle;
    const float angle = rotaryStartAngle + sliderPosProportional * angleRange;

    // --- (1) Draw the outer ring for style ---
    juce::ColourGradient ringGradient(juce::Colours::lightgrey, centre.x, centre.y,
        juce::Colours::darkgrey, centre.x, centre.y, true);
    ringGradient.addColour(0.5, juce::Colours::grey);
    g.setGradientFill(ringGradient);

    const float ringRadius = radius + 5.0f;
    g.fillEllipse(centre.x - ringRadius, centre.y - ringRadius, ringRadius * 2.0f, ringRadius * 2.0f);

    // --- (2) Subtle inner circle ---
    g.setColour(juce::Colours::black.withAlpha(0.2f));
    g.fillEllipse(centre.x - radius, centre.y - radius, radius * 2.0f, radius * 2.0f);

    // --- (3) Draw the main value arc ---
    const float arcThickness = 6.0f;
    const float arcRadius = radius + 1.0f;

    juce::Path valueArc;
    valueArc.addArc(centre.x - arcRadius,
        centre.y - arcRadius,
        arcRadius * 2.0f,
        arcRadius * 2.0f,
        rotaryStartAngle, angle, true);

    g.setColour(juce::Colours::orange);
    g.strokePath(valueArc, juce::PathStrokeType(arcThickness,
        juce::PathStrokeType::curved,
        juce::PathStrokeType::rounded));

    // --- (4) White indicator at arc’s end ---
    // Instead of pushing it outward by half the arc thickness, we place it right on the arc’s center.
    const float effectiveRadius = arcRadius;
    const float indicatorRadius = 4.0f;

    // Convert 'angle' into XY coords at the arc center-line:
    float indicatorX = centre.x + effectiveRadius * std::cos(angle);
    float indicatorY = centre.y + effectiveRadius * std::sin(angle);

    g.setColour(juce::Colours::white);
    g.fillEllipse(indicatorX - indicatorRadius,
        indicatorY - indicatorRadius,
        indicatorRadius * 2.0f,
        indicatorRadius * 2.0f);

    // --- (5) Centre circle (small black dot) ---
    g.setColour(juce::Colours::black);
    g.fillEllipse(centre.x - 3.0f, centre.y - 3.0f, 6.0f, 6.0f);

    // --- (6) Knob’s current value in the centre ---
    const double currentValue = slider.getValue();
    juce::String valueText;
    if (std::abs(slider.getRange().getLength()) < 10.0)
        valueText = juce::String(currentValue, 2);
    else
        valueText = juce::String((int)currentValue);

    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(12.0f, juce::Font::bold));

    juce::Rectangle<float> textRect(centre.x - 20.0f, centre.y - 10.0f, 40.0f, 20.0f);
    g.drawFittedText(valueText, textRect.toNearestInt(), juce::Justification::centred, 1);

    // --- (7) Tick Marks around the knob ---
    drawTickMarks(g, centre.x, centre.y, radius, rotaryStartAngle, angleRange, slider);
}


void KnobLookAndFeel::drawTickMarks(juce::Graphics& g,
    float centreX, float centreY, float radius,
    float startAngle, float angleRange,
    juce::Slider& slider)
{
    // Example: 7 tick marks
    const int numTicks = 7;

    for (int i = 0; i < numTicks; ++i)
    {
        float fraction = (float)i / (float)(numTicks - 1);
        float thisAngle = startAngle + fraction * angleRange;

        // Outer/inner coords for the tick line
        float tickOuterX = centreX + radius * std::cos(thisAngle);
        float tickOuterY = centreY + radius * std::sin(thisAngle);
        float tickLength = 5.0f;
        float tickInnerX = centreX + (radius - tickLength) * std::cos(thisAngle);
        float tickInnerY = centreY + (radius - tickLength) * std::sin(thisAngle);

        g.setColour(juce::Colours::grey);
        g.drawLine(tickOuterX, tickOuterY, tickInnerX, tickInnerY, 1.0f);

        // Position the numeric label further out
        const float labelRadius = radius + 30.0f;
        float labelX = centreX + labelRadius * std::cos(thisAngle);
        float labelY = centreY + labelRadius * std::sin(thisAngle);

        double tickValue = slider.getRange().getStart() + fraction * slider.getRange().getLength();

        // Format label
        juce::String labelText;
        if (std::abs(slider.getRange().getLength()) < 10.0)
            labelText = juce::String(tickValue, 2);
        else
            labelText = juce::String((int)tickValue);

        g.setColour(juce::Colours::white);
        g.setFont(10.0f);

        juce::Rectangle<float> textArea(labelX - 20.0f, labelY - 10.0f, 40.0f, 20.0f);
        g.drawFittedText(labelText, textArea.toNearestInt(), juce::Justification::centred, 1);
    }
}
