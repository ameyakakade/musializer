#include "PluginProcessor.h"
#include "PluginEditor.h"

int height = 400;
int width  = 400;

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    setSize (height, width);
    startTimer(16);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    float m = processorRef.fft_analyze(0.016f);
    int step = width/m;
    g.setColour (juce::Colours::white);
    for(size_t i=0; i<m-1; i++)
    {
        float t = processorRef.out_smooth[i];
        g.fillRect((float)(i*step) , 0.0, (float)step/2, t*2/3*height);
    }
}

void AudioPluginAudioProcessorEditor::timerCallback()
{
    repaint();
}

void AudioPluginAudioProcessorEditor::resized()
{
}
