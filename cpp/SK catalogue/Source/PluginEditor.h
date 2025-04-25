/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SKcatalogueAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SKcatalogueAudioProcessorEditor (SKcatalogueAudioProcessor&);
    ~SKcatalogueAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SKcatalogueAudioProcessor& audioProcessor;
    
    juce::Colour mBackgroundColour;
    juce::Label pluginLabel {"SK CATALOGUE"};
    
    juce::Slider cutoffSlider;
    juce::Label cutoffLabel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SKcatalogueAudioProcessorEditor)
};
