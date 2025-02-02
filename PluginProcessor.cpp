#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JuceSimpleGainReductionAudioProcessor::JuceSimpleGainReductionAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
#endif
{
}

JuceSimpleGainReductionAudioProcessor::~JuceSimpleGainReductionAudioProcessor()
{
}

//==============================================================================
const juce::String JuceSimpleGainReductionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JuceSimpleGainReductionAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool JuceSimpleGainReductionAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool JuceSimpleGainReductionAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double JuceSimpleGainReductionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JuceSimpleGainReductionAudioProcessor::getNumPrograms()
{
    return 1;
}

int JuceSimpleGainReductionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JuceSimpleGainReductionAudioProcessor::setCurrentProgram(int /*index*/)
{
}

const juce::String JuceSimpleGainReductionAudioProcessor::getProgramName(int /*index*/)
{
    return {};
}

void JuceSimpleGainReductionAudioProcessor::changeProgramName(int /*index*/, const juce::String& /*newName*/)
{
}

//==============================================================================
void JuceSimpleGainReductionAudioProcessor::prepareToPlay(double newSampleRate, int /*samplesPerBlock*/)
{
    sampleRate = newSampleRate;

    // Initialize per-channel state.
    auto numChannels = getTotalNumInputChannels();
    smoothedGain.clear();
    smoothedGain.resize(numChannels, 1.0f);
    envelope.clear();
    envelope.resize(numChannels, 0.0f);
}

void JuceSimpleGainReductionAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JuceSimpleGainReductionAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
        layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void JuceSimpleGainReductionAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that have no input data.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    float attackTimeSec = attackMs / 1000.0f;
    float releaseTimeSec = releaseMs / 1000.0f;

    float envAttackCoeff = std::exp(-1.0f / (attackTimeSec * static_cast<float>(sampleRate)));
    float envReleaseCoeff = std::exp(-1.0f / (releaseTimeSec * static_cast<float>(sampleRate)));

    float makeupGainLinear = juce::Decibels::decibelsToGain(makeupGain);
    float maxReductionDB = 0.0f;

    if (smoothedGain.size() < static_cast<size_t>(totalNumInputChannels))
        smoothedGain.resize(totalNumInputChannels, 1.0f);
    if (envelope.size() < static_cast<size_t>(totalNumInputChannels))
        envelope.resize(totalNumInputChannels, 0.0f);

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float inSample = channelData[sample];
            float inputAbs = std::abs(inSample);

            if (inputAbs > envelope[channel])
                envelope[channel] = envAttackCoeff * envelope[channel] + (1.0f - envAttackCoeff) * inputAbs;
            else
                envelope[channel] = envReleaseCoeff * envelope[channel] + (1.0f - envReleaseCoeff) * inputAbs;

            float levelDB = juce::Decibels::gainToDecibels(envelope[channel], -100.0f);

            float desiredReductionDB = 0.0f;
            if (kneeDB > 0.0f)
            {
                if (levelDB < thresholdDB - kneeDB * 0.5f)
                {
                    desiredReductionDB = 0.0f;
                }
                else if (levelDB > thresholdDB + kneeDB * 0.5f)
                {
                    desiredReductionDB = (levelDB - thresholdDB) * (1.0f - 1.0f / ratio);
                }
                else
                {
                    float delta = levelDB - (thresholdDB - kneeDB * 0.5f);
                    desiredReductionDB = (1.0f - 1.0f / ratio) * (delta * delta) / (2.0f * kneeDB);
                }
            }
            else
            {
                if (levelDB > thresholdDB)
                    desiredReductionDB = (levelDB - thresholdDB) * (1.0f - 1.0f / ratio);
            }

            float desiredGainDB = -desiredReductionDB;
            float desiredGain = juce::Decibels::decibelsToGain(desiredGainDB);

            float gainCoeff = (desiredGain < smoothedGain[channel])
                ? std::exp(-1.0f / (attackTimeSec * static_cast<float>(sampleRate)))
                : std::exp(-1.0f / (releaseTimeSec * static_cast<float>(sampleRate)));
            smoothedGain[channel] = desiredGain + (smoothedGain[channel] - desiredGain) * gainCoeff;

            float outSample = inSample * smoothedGain[channel] * makeupGainLinear;
            channelData[sample] = outSample;

            if (desiredReductionDB > maxReductionDB)
                maxReductionDB = desiredReductionDB;
        }
    }

    gainReduction = maxReductionDB;
}

//==============================================================================
bool JuceSimpleGainReductionAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* JuceSimpleGainReductionAudioProcessor::createEditor()
{
    return new JuceSimpleGainReductionAudioProcessorEditor(*this);
}

//==============================================================================
void JuceSimpleGainReductionAudioProcessor::getStateInformation(juce::MemoryBlock& /*destData*/)
{
    // Save the plugin's state (parameters) here.
}

void JuceSimpleGainReductionAudioProcessor::setStateInformation(const void* /*data*/, int /*sizeInBytes*/)
{
    // Restore the plugin's state (parameters) here.
}

//==============================================================================
void JuceSimpleGainReductionAudioProcessor::setThresholdDB(float newThresholdDB)
{
    thresholdDB = newThresholdDB;
}

void JuceSimpleGainReductionAudioProcessor::setRatio(float newRatio)
{
    ratio = newRatio;
}

void JuceSimpleGainReductionAudioProcessor::setAttackMs(float newAttackMs)
{
    attackMs = newAttackMs;
}

void JuceSimpleGainReductionAudioProcessor::setReleaseMs(float newReleaseMs)
{
    releaseMs = newReleaseMs;
}

void JuceSimpleGainReductionAudioProcessor::setMakeupGain(float newMakeupGain)
{
    makeupGain = newMakeupGain;
}

void JuceSimpleGainReductionAudioProcessor::setKneeDB(float newKneeDB)
{
    kneeDB = newKneeDB;
}

void JuceSimpleGainReductionAudioProcessor::setKeyFilterFreq(float newKeyFilterFreq)
{
    keyFilterFreq = newKeyFilterFreq;
}

double& JuceSimpleGainReductionAudioProcessor::getGainReduction()
{
    return gainReduction;
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JuceSimpleGainReductionAudioProcessor();
}
