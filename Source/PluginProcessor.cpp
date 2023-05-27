/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ExpanderAudioProcessor::ExpanderAudioProcessor()

#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                       lleno(false),
                       state(false),
                       thresholdE(-10),
                       attackE(100e-3),
                       releaseE(10e-3),
                       ratioE(2),
                       ratioEU(0.02),
                       makeupE(0)
                       
#endif
{
  



}

ExpanderAudioProcessor::~ExpanderAudioProcessor()
{
}

//==============================================================================
const juce::String ExpanderAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ExpanderAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ExpanderAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ExpanderAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ExpanderAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ExpanderAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ExpanderAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ExpanderAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ExpanderAudioProcessor::getProgramName (int index)
{
    return {};
}

void ExpanderAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ExpanderAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    CompresionL.setSamplerate(sampleRate);
    CompresionR.setSamplerate(sampleRate);
    
    CompresionUL.setSamplerate(sampleRate);
    CompresionUR.setSamplerate(sampleRate);

    rmsLevelL.reset(sampleRate, 0.5);
    rmsLevelR.reset(sampleRate, 0.5);

    rmsLevelL.setCurrentAndTargetValue(-100.f);
    rmsLevelR.setCurrentAndTargetValue(-100.f);
    
    //residuoE.reset(sampleRate, 0.5);
    //residuoU.reset(sampleRate, 0.5);

    //residuoE.setCurrentAndTargetValue(-60.f);
    //residuoU.setCurrentAndTargetValue(-60.f);

}

void ExpanderAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ExpanderAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ExpanderAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    rmsLevelL.skip(buffer.getNumSamples());
    rmsLevelR.skip(buffer.getNumSamples());
    {
        const auto valueL = juce::Decibels::gainToDecibels(buffer.getRMSLevel(0, 0, buffer.getNumSamples()));
        if (valueL < rmsLevelL.getCurrentValue())
            rmsLevelL.setTargetValue(valueL);
        else
            rmsLevelL.setCurrentAndTargetValue(valueL);
    
    }
    {
        const auto valueR = juce::Decibels::gainToDecibels(buffer.getRMSLevel(1, 0, buffer.getNumSamples()));
        if (valueR < rmsLevelR.getCurrentValue())
            rmsLevelR.setTargetValue(valueR);
        else
            rmsLevelR.setCurrentAndTargetValue(valueR);
    }
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    

    CompresionL.setattack(attackE);
    CompresionR.setattack(attackE);
    CompresionUL.setattack(attackE);
    CompresionUR.setattack(attackE);

    CompresionL.setrelease(releaseE);
    CompresionR.setrelease(releaseE);
    CompresionUL.setrelease(releaseE);
    CompresionUR.setrelease(releaseE);


    CompresionL.settreshold(thresholdE);
    CompresionR.settreshold(thresholdE);
    CompresionUL.settreshold(thresholdE);
    CompresionUR.settreshold(thresholdE);

    CompresionL.setratio(ratioE, "Down");
    CompresionR.setratio(ratioE, "Down");
    CompresionUL.setratio(ratioEU, "Up");
    CompresionUR.setratio(ratioEU, "Up");

   

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());


    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {

        

        for (int channel = 0; channel < totalNumInputChannels; channel++)
        {
            float* SampleN = buffer.getWritePointer(channel);
            
            if (channel == 0)
            {
                if (state == false)
                {
                    SampleN[sample] = makeupE * CompresionL.procesSampleD(SampleN[sample]);
                }
                else if (state == true)
                {
                    SampleN[sample] =  makeupE * CompresionUL.processSampleU(SampleN[sample]);
                }
                
                

                
            }

            else if (channel == 1)
            {
                
                if (state == false)
                {
                    SampleN[sample] = makeupE * CompresionR.procesSampleD(SampleN[sample]);
                }
                else if (state == true)
                {
                    SampleN[sample] = makeupE * CompresionUR.processSampleU(SampleN[sample]);
                }
                
                
            }

        }

    }
    

}

//==============================================================================
bool ExpanderAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ExpanderAudioProcessor::createEditor()
{
    return new ExpanderAudioProcessorEditor (*this);
}

//==============================================================================
void ExpanderAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ExpanderAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ExpanderAudioProcessor();
}
