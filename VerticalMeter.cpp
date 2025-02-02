#include "VerticalMeter.h"

VerticalMeter::VerticalMeter() {}
VerticalMeter::~VerticalMeter() {}

void VerticalMeter::setGainReduction(float dBValue)
{
    dBValue = juce::jlimit(-60.0f, 0.0f, dBValue);
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
    auto bounds = getLocalBounds().toFloat();
    g.fillAll(juce::Colours::black);

    // from -60 dB to 0 dB
    float range = 60.0f;
    float fraction = 1.0f - ((currentValue + 60.0f) / range); // 0.0 at 0 dB, 1.0 at -60 dB
    fraction = juce::jlimit(0.0f, 1.0f, fraction);

    // fill region
    auto meterHeight = bounds.getHeight() * fraction;
    auto fillArea = bounds.removeFromBottom(meterHeight);
    juce::ColourGradient cg(juce::Colours::green,
        fillArea.getCentreX(), fillArea.getY(),
        juce::Colours::red,
        fillArea.getCentreX(), fillArea.getBottom(),
        false);
    g.setGradientFill(cg);
    g.fillRect(fillArea);

    // draw outline
    g.setColour(juce::Colours::grey);
    g.drawRect(bounds.expanded(0.0f, meterHeight), 1.0f);

    // Optional: draw tick marks e.g. -3, -6, etc.
    // Implementation left to user – see the earlier snippet if needed!
}
