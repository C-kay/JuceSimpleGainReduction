#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
    This processor implements an industry?standard gain reduction (compressor)
    with envelope detection, soft knee, and separate attack/release smoothing.
*/
class JuceSimpleGainReductionAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    JuceSimpleGainReductionAudioProcessor();
    ~JuceSimpleGainReductionAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // Parameter setters (for GUI control)
    void setThresholdDB(float newThresholdDB);
    void setRatio(float newRatio);
    void setAttackMs(float newAttackMs);
    void setReleaseMs(float newReleaseMs);
    void setMakeupGain(float newMakeupGain);
    void setKneeDB(float newKneeDB); // 0 = hard knee, > 0 = soft knee width in dB
    void setKeyFilterFreq(float newKeyFilterFreq);

    // Getter for meter display (gain reduction in dB)
    double& getGainReduction();

private:
    //==============================================================================
    // Compressor parameters
    float thresholdDB{ -24.0f };   // dB threshold for compression
    float ratio{ 4.0f };      // Compression ratio
    float attackMs{ 10.0f };     // Attack time in milliseconds
    float releaseMs{ 100.0f };    // Release time in milliseconds
    float makeupGain{ 0.0f };      // Makeup gain in dB
    float kneeDB{ 0.0f };      // Knee width in dB (0 = hard knee)
    float keyFilterFreq{ 1000.0f }; // (Optional) key filter frequency for sidechain

    // Computed gain reduction for display (in dB)
    double gainReduction{ 0.0 };

    // Per?channel state:
    // – smoothedGain: stores the currently applied linear gain factor (after compression/smoothing)
    // – envelope: tracks the signal’s amplitude (with attack and release smoothing)
    std::vector<float> smoothedGain;
    std::vector<float> envelope;

    // Sample rate (set in prepareToPlay)
    double sampleRate{ 44100.0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JuceSimpleGainReductionAudioProcessor)
};
