#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "KnobLookAndFeel.h"
#include "VerticalMeter.h"

class JuceSimpleGainReductionAudioProcessorEditor
    : public juce::AudioProcessorEditor,
    private juce::Slider::Listener,
    private juce::Timer
{
public:
    JuceSimpleGainReductionAudioProcessorEditor(JuceSimpleGainReductionAudioProcessor&);
    ~JuceSimpleGainReductionAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    JuceSimpleGainReductionAudioProcessor& audioProcessor;

    // Sliders + Labels
    juce::Slider gainReductionSlider, thresholdDBSlider, ratioSlider;
    juce::Slider attackMsSlider, releaseMsSlider, makeupGainSlider;
    juce::Slider keyFilterFreqSlider;

    juce::Label gainReductionLabel, thresholdDBLabel, ratioLabel;
    juce::Label attackMsLabel, releaseMsLabel, makeupGainLabel;
    juce::Label keyFilterFreqLabel;

    // The custom meter
    VerticalMeter verticalMeter;

    // The custom knob look+feel
    KnobLookAndFeel knobLnf;

    void sliderValueChanged(juce::Slider* slider) override;
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JuceSimpleGainReductionAudioProcessorEditor)
};
