#include "PluginEditor.h"
#include "PluginProcessor.h"

JuceSimpleGainReductionAudioProcessorEditor::JuceSimpleGainReductionAudioProcessorEditor
(JuceSimpleGainReductionAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    // Set plugin window size.
    setSize(600, 300);

    // Lambda to initialize interactive sliders.
    auto initSlider = [this](juce::Slider& s, juce::Label& label,
        const juce::String& labelText, double minVal, double maxVal, double defaultVal)
        {
            s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
            s.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
            s.setLookAndFeel(&knobLnf);
            s.setRange(minVal, maxVal, 0.01);
            s.setValue(defaultVal);
            s.addListener(this);
            addAndMakeVisible(s);

            label.setText(labelText, juce::dontSendNotification);
            label.setJustificationType(juce::Justification::centred);
            addAndMakeVisible(label);
        };

    // Initialize the meter slider separately (as a display-only control).
    gainReductionSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainReductionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    gainReductionSlider.setLookAndFeel(&knobLnf);
    gainReductionSlider.setRange(0.0, 1.0, 0.01);
    gainReductionSlider.setValue(1.0);
    gainReductionSlider.setEnabled(false); // Disable user interaction.
    addAndMakeVisible(gainReductionSlider);
    gainReductionLabel.setText("GainReduction", juce::dontSendNotification);
    gainReductionLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(gainReductionLabel);

    // Initialize the other interactive sliders.
    initSlider(thresholdDBSlider, thresholdDBLabel, "Threshold (dB)", -60.0, 0.0, -24.0);
    initSlider(ratioSlider, ratioLabel, "Ratio", 1.0, 20.0, 4.0);
    initSlider(attackMsSlider, attackMsLabel, "Attack (ms)", 1.0, 100.0, 10.0);
    initSlider(releaseMsSlider, releaseMsLabel, "Release (ms)", 10.0, 500.0, 100.0);
    initSlider(makeupGainSlider, makeupGainLabel, "Makeup Gain", 0.0, 24.0, 0.0);
    initSlider(keyFilterFreqSlider, keyFilterFreqLabel, "KeyFilterFreq", 20.0, 20000.0, 1000.0);

    // Add the vertical meter component.
    addAndMakeVisible(verticalMeter);

    // Start the timer to update the meter (30 Hz).
    startTimerHz(30);
}

JuceSimpleGainReductionAudioProcessorEditor::~JuceSimpleGainReductionAudioProcessorEditor()
{
    // Clear the LookAndFeel pointers to avoid dangling references.
    gainReductionSlider.setLookAndFeel(nullptr);
    thresholdDBSlider.setLookAndFeel(nullptr);
    ratioSlider.setLookAndFeel(nullptr);
    attackMsSlider.setLookAndFeel(nullptr);
    releaseMsSlider.setLookAndFeel(nullptr);
    makeupGainSlider.setLookAndFeel(nullptr);
    keyFilterFreqSlider.setLookAndFeel(nullptr);
    gainReductionSlider.setLookAndFeel(nullptr); // already cleared above
}

void JuceSimpleGainReductionAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff303030)); // Dark grey background.
}

void JuceSimpleGainReductionAudioProcessorEditor::resized()
{
    // Layout: 2 rows of knobs and a meter on the right.
    auto area = getLocalBounds().reduced(10);

    // Reserve a vertical strip on the right for the meter.
    auto meterWidth = 60;
    auto meterArea = area.removeFromRight(meterWidth);
    verticalMeter.setBounds(meterArea);

    // Top row: 4 knobs (GainReduction meter, Threshold, Ratio, Attack).
    auto topRow = area.removeFromTop(area.getHeight() * 0.5f);
    auto knobWidth = topRow.getWidth() / 4;

    auto placeKnob = [&](juce::Slider& s, juce::Label& lbl, juce::Rectangle<int> r)
        {
            // Make the knob square and place the label below.
            s.setBounds(r.withSizeKeepingCentre(r.getHeight(), r.getHeight()).reduced(5));
            lbl.setBounds(r.removeFromBottom(20));
        };

    {
        auto r1 = topRow.removeFromLeft(knobWidth);
        placeKnob(gainReductionSlider, gainReductionLabel, r1);

        auto r2 = topRow.removeFromLeft(knobWidth);
        placeKnob(thresholdDBSlider, thresholdDBLabel, r2);

        auto r3 = topRow.removeFromLeft(knobWidth);
        placeKnob(ratioSlider, ratioLabel, r3);

        auto r4 = topRow; // Remaining area.
        placeKnob(attackMsSlider, attackMsLabel, r4);
    }

    // Bottom row: 3 knobs (Release, Makeup Gain, Key Filter).
    auto knobWidthBottom = area.getWidth() / 3;
    {
        auto r1 = area.removeFromLeft(knobWidthBottom);
        placeKnob(releaseMsSlider, releaseMsLabel, r1);

        auto r2 = area.removeFromLeft(knobWidthBottom);
        placeKnob(makeupGainSlider, makeupGainLabel, r2);

        auto r3 = area;  // Remaining area.
        placeKnob(keyFilterFreqSlider, keyFilterFreqLabel, r3);
    }
}

void JuceSimpleGainReductionAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    // Only process changes from interactive sliders.
    if (slider == &thresholdDBSlider)
        audioProcessor.setThresholdDB((float)slider->getValue());
    else if (slider == &ratioSlider)
        audioProcessor.setRatio((float)slider->getValue());
    else if (slider == &attackMsSlider)
        audioProcessor.setAttackMs((float)slider->getValue());
    else if (slider == &releaseMsSlider)
        audioProcessor.setReleaseMs((float)slider->getValue());
    else if (slider == &makeupGainSlider)
        audioProcessor.setMakeupGain((float)slider->getValue());
    else if (slider == &keyFilterFreqSlider)
        audioProcessor.setKeyFilterFreq((float)slider->getValue());
}

void JuceSimpleGainReductionAudioProcessorEditor::timerCallback()
{
    // Update the meter based on the processor’s computed gain reduction (in dB).
    float grDb = audioProcessor.getGainReduction();
    verticalMeter.setGainReduction(grDb);
}
