/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SKcatalogueAudioProcessor::SKcatalogueAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParams())
#endif
{
}

SKcatalogueAudioProcessor::~SKcatalogueAudioProcessor()
{
}

//==============================================================================
const juce::String SKcatalogueAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SKcatalogueAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SKcatalogueAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SKcatalogueAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SKcatalogueAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SKcatalogueAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SKcatalogueAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SKcatalogueAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SKcatalogueAudioProcessor::getProgramName (int index)
{
    return {};
}

void SKcatalogueAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SKcatalogueAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    mSampleRate = sampleRate;
    mSamplesPerBlock = samplesPerBlock;
    mNumChannels = getTotalNumOutputChannels();
    
    // Frees up the heap memory
    for (auto i = 0; i < sk_lpf.size(); i++)
    {
        delete sk_lpf[i];
    }
    
    sk_lpf.clear();
    
    // Initializes processors
    for (auto i = 0; i < mNumChannels; i++)
    {
        sk_lpf.push_back(new SallenKeyLPF);
        sk_lpf[i]->prepare(mSampleRate);
    }
    
}

void SKcatalogueAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    
    sk_lpf.clear();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SKcatalogueAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SKcatalogueAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    // channel processing
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {

        // set parameters
        auto& cutoff = *apvts.getRawParameterValue("cutoff");
        // DBG(">>> new cutoff: " << cutoff);
        sk_lpf[channel]->setParams(cutoff, 1);
        
        // prepare buffer pointers for reading and writing
        const float* x = buffer.getReadPointer (channel);
        float* y = buffer.getWritePointer(channel);
        
        // sample processing
        for (int j = 0; j < buffer.getNumSamples(); j++)
        {
            float inputSample = x[j];
            y[j] = sk_lpf[channel]->processSample(inputSample);
        }

    }
}

//==============================================================================
bool SKcatalogueAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SKcatalogueAudioProcessor::createEditor()
{
    return new SKcatalogueAudioProcessorEditor (*this);
}

//==============================================================================
void SKcatalogueAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SKcatalogueAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SKcatalogueAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout SKcatalogueAudioProcessor::createParams()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    juce::NormalisableRange<float> cutoffRange (20.0f, 16000.0f, .1f);
    cutoffRange.setSkewForCentre(1000.0f);
    
    params.add(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID("cutoff", 1), "Cutoff", cutoffRange, 1000.0f));
    return params;
}

void SKcatalogueAudioProcessor::parameterChanged(const juce::String &parameterID, float newValue)
{
    /*auto totalNumInputChannels = getTotalNumInputChannels();
    
    if (parameterID == "cutoff")
    {
        auto* cutoffParam = dynamic_cast<juce::AudioParameterFloat>(apvts.getParameter("cutoff"));
    }*/
}
