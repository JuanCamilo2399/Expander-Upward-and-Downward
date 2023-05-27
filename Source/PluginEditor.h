/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ImageKnob.h"
#include "SpectrumClass.h"
#include "Level Meter.h"
#include "Curve.h"


//==============================================================================
/**
*/
class ExpanderAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                      private juce::Timer
{
public:
    ExpanderAudioProcessorEditor (ExpanderAudioProcessor&);
    ~ExpanderAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    
    HorizontalMeter MeterL, MeterR;
    

    Curve charCurve;
    //SpectrumClass spectrumAnalizer;

    juce::Image Imagen;
    juce::Image imagenFondo;
    juce::Image knobImage;
    juce::Image knob2Image;
    juce::Image SwitchUp;
    juce::Image SwitchDown;

    std::unique_ptr<ImageKnob> knob;
    std::unique_ptr<ImageKnob> knobU;
    std::unique_ptr<ImageKnob> knob1;
    std::unique_ptr<ImageKnob> knob2;
    std::unique_ptr<ImageKnob> knob3;
    std::unique_ptr<ImageKnob> knob4;

    juce::Label ratioLabel, attacklabel, releaselabel, thresholdlabel, makeuplabel;
    
    juce::ImageButton Switch;
    

    ExpanderAudioProcessor& audioProcessor;

    std::vector<float> Ratiovalue{2.000f,4.000f,8.000f,12.000f,100.000f};
    std::vector<float> RatiovalueU{ 0.25f,0.35f,0.5f,0.75f,0.88f };
    std::vector<float> AttackValues;
    std::vector<float>releaseValues;
    std::vector<float>ThresholdValues;
    std::vector<float>MakeupValues;

    void setlabel(juce::Label& Label, juce::String text);
    void Logaritmic(std::vector<float>& vector,float start, float stop, int numvalues);
    void SetKnobDecibels(std::vector<float>& vector, float start, float mid, float end);
    

    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExpanderAudioProcessorEditor)
};
