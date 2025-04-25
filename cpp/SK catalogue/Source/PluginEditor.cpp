/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SKcatalogueAudioProcessorEditor::SKcatalogueAudioProcessorEditor (SKcatalogueAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    // define a background color
    mBackgroundColour = juce::Colour::fromRGB(128, 0, 128);
    
    // plugin label
    pluginLabel.setText("SALLEN KEY CATALOGUE", juce::dontSendNotification);
    addAndMakeVisible(pluginLabel);
    
    // set the cutoff slider
    cutoffSlider.setTextBoxStyle (juce::Slider::TextBoxLeft, false, 160, cutoffSlider.getTextBoxHeight());
    cutoffSlider.setTextValueSuffix (" Hz");
    addAndMakeVisible(cutoffSlider);
    cutoffSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "cutoff", cutoffSlider);
    
    addAndMakeVisible(cutoffLabel);
    cutoffLabel.setText("cutoff", juce::dontSendNotification);
    cutoffLabel.attachToComponent(&cutoffSlider, true);
    
}

SKcatalogueAudioProcessorEditor::~SKcatalogueAudioProcessorEditor()
{
}

//==============================================================================
void SKcatalogueAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll (mBackgroundColour);

    /*
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    */
}

void SKcatalogueAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    auto area = getLocalBounds();
    auto width = area.getWidth();
    auto halfWidth = width / 2;
    auto height = area.getHeight();
    
    pluginLabel.setBounds(halfWidth - 75, 10, width, 20);
    
    auto sliderLeft = 120;
    
    cutoffSlider.setBounds(sliderLeft, 40, width - sliderLeft - 10, 20);
    
    // Another way using rectangles and areas
    /*
    auto headerRect = area.removeFromTop(50);
    auto pluginLabelArea = area.removeFromTop(100);
    
    pluginLabel.setBounds(pluginLabelArea);
    
    auto cutoffSliderArea = area.removeFromTop(100);
    cutoffSlider.setBounds(cutoffSliderArea);
     */
    
}
