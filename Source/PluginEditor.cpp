/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ExpanderAudioProcessorEditor::ExpanderAudioProcessorEditor (ExpanderAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), 
    MeterL([&]() { return audioProcessor.rmsLevelL.getCurrentValue(); }),
    MeterR([&]() { return audioProcessor.rmsLevelR.getCurrentValue(); })
   
   
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
   Imagen = juce::ImageFileFormat::loadFrom(BinaryData::Fondo_Expander_png, BinaryData::Fondo_Expander_pngSize);
   knobImage = juce::ImageFileFormat::loadFrom(BinaryData::KnobsART_png, BinaryData::KnobsART_pngSize);
   knob2Image = juce::ImageFileFormat::loadFrom(BinaryData::KnobRatio_png, BinaryData::KnobRatio_pngSize);

   SwitchUp = juce::ImageFileFormat::loadFrom(BinaryData::modo_perilla_01_png, BinaryData::modo_perilla_01_pngSize);
   SwitchDown = juce::ImageFileFormat::loadFrom(BinaryData::modo_perilla_03_png, BinaryData::modo_perilla_03_pngSize);

   addAndMakeVisible(Switch);
   Switch.setImages(false, false, true,
       SwitchUp, 1.0f, {},
       juce::Image(), 1.0f, {},
       SwitchDown, 1.0f, {});
   Switch.setToggleable(true);
   Switch.setToggleState(false, juce::dontSendNotification);
   Switch.onClick = [this]
   {
       Switch.setToggleState(!Switch.getToggleState(), juce::dontSendNotification);
       audioProcessor.state = Switch.getToggleState();
       charCurve.setstate(Switch.getToggleState());
       if (audioProcessor.state)
       {
           knob->setVisible(false);
           knobU->setVisible(true);
       }
       else 
       {
           knob->setVisible(true);
           knobU->setVisible(false); 
       }
       
   };

   addAndMakeVisible(ratioLabel);
   ratioLabel.setFont(juce::Font(16.0f));

   addAndMakeVisible(attacklabel);
   attacklabel.setFont(juce::Font(16.0f));

   addAndMakeVisible(releaselabel);
   releaselabel.setFont(juce::Font(16.0f));

   addAndMakeVisible(thresholdlabel);
   thresholdlabel.setFont(juce::Font(16.0f));

   addAndMakeVisible(makeuplabel);
   makeuplabel.setFont(juce::Font(16.0f));

   Logaritmic(AttackValues, 0.100, 1, 60);
   Logaritmic(releaseValues, 0.020, 5, 60);
   SetKnobDecibels(ThresholdValues, -60, -30, 0);
   ThresholdValues[59] = 0;
   SetKnobDecibels(MakeupValues, -40, 0, 10);
   MakeupValues[30] = 0;

   knob.reset(new ImageKnob(knob2Image, 5, false));
   addAndMakeVisible(knob.get());
   knob->setRange(0, 4, 1);
   knob->setValue(0);
   knob->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
   knob->setVisible(true);

   knob->onValueChange = [this]
   {
       ratioLabel.setText(juce::String(Ratiovalue[knob->getValue()]) + ":1", juce::dontSendNotification);
       ratioLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::whitesmoke);
       ratioLabel.setJustificationType(juce::Justification::centred);
       audioProcessor.ratioE = Ratiovalue[knob->getValue()];
       charCurve.comp.setratio(Ratiovalue[knob->getValue()], "Down");
   };


   knobU.reset(new ImageKnob(knob2Image, 5, false));
   addAndMakeVisible(knobU.get());
   knobU->setRange(0, 4, 1);
   knobU->setValue(0);
   knobU->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
   knobU->setVisible(false);

   knobU->onValueChange = [this]
   {
       ratioLabel.setText(juce::String(RatiovalueU[knobU->getValue()]) + ":1", juce::dontSendNotification);
       ratioLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::whitesmoke);
       ratioLabel.setJustificationType(juce::Justification::centred);
       audioProcessor.ratioEU = RatiovalueU[knobU->getValue()];
       charCurve.comp.setratio(RatiovalueU[knobU->getValue()], "Up");
   };

   knob1.reset(new ImageKnob(knobImage, 60, false));
   addAndMakeVisible(knob1.get());
   knob1->setRange(0, 59,1);
   knob1->setValue(1);
   knob1->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
   knob1->onValueChange = [this]
   {
       attacklabel.setText(juce::String(std::round(1000*AttackValues[knob1->getValue()])) + "" + "ms", juce::dontSendNotification);
       attacklabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::whitesmoke);
       attacklabel.setJustificationType(juce::Justification::centred);
       audioProcessor.attackE =AttackValues[knob1->getValue()];
       charCurve.comp.setattack(AttackValues[knob1->getValue()]);
   };


   knob2.reset(new ImageKnob(knobImage, 60, false));
   addAndMakeVisible(knob2.get());
   knob2->setRange(0, 59,1);
   knob2->setValue(1);
   knob2->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
   knob2->onValueChange = [this]
   {
       releaselabel.setText(juce::String(std::round(1000 * releaseValues[knob2->getValue()])) + "" + "ms", juce::dontSendNotification);
       releaselabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::whitesmoke);
       releaselabel.setJustificationType(juce::Justification::centred);
       audioProcessor.releaseE = releaseValues[knob2->getValue()];
       charCurve.comp.setrelease(releaseValues[knob2->getValue()]);
   };
    
   knob3.reset(new ImageKnob(knobImage, 60, false));
   addAndMakeVisible(knob3.get());
   knob3->setRange(0,59,1);
   knob3->setValue(30);
   knob3->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
   knob3->onValueChange = [this]
   {
       thresholdlabel.setText(juce::String(ThresholdValues[knob3->getValue()]) + "" + "dB", juce::dontSendNotification);
       thresholdlabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::whitesmoke);
       thresholdlabel.setJustificationType(juce::Justification::centred);
       audioProcessor.thresholdE = ThresholdValues[knob3->getValue()];
       charCurve.comp.settreshold(ThresholdValues[knob3->getValue()]);
   };

   knob4.reset(new ImageKnob(knobImage, 60, false));
   addAndMakeVisible(knob4.get());
   knob4->setRange(0, 59,1);
   knob4->setValue(30);
   
   knob4->setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
   knob4->onValueChange = [this]
   {
       makeuplabel.setText(juce::String(std::round(MakeupValues[knob4->getValue()])) + "" + "dB", juce::dontSendNotification);
       makeuplabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::whitesmoke);
       makeuplabel.setJustificationType(juce::Justification::centred);
       audioProcessor.makeupE = juce::Decibels::decibelsToGain(MakeupValues[knob4->getValue()]);
   }; 

   MeterL.state = true;
   MeterR.state = true;
   
   
   

   addAndMakeVisible(MeterL);
   addAndMakeVisible(MeterR);
   //addAndMakeVisible(MeterRedu);
   addAndMakeVisible(charCurve);


   startTimerHz(60);
   setSize(1000, 900);


}

ExpanderAudioProcessorEditor::~ExpanderAudioProcessorEditor()
{
    
}

//==============================================================================
void ExpanderAudioProcessorEditor::paint (juce::Graphics& g)
{
    juce::Rectangle<int> area = getLocalBounds();

    g.drawImage(Imagen, area.toFloat());
}

void ExpanderAudioProcessorEditor::resized()
{

    const auto labelOffset = 75;
    const auto labelOffset2 = 55;
    const auto paddingx = 160;
    const auto paddingy = 680;

    const auto paddingMeter = 142;

    knob->setBounds(paddingx, paddingy , 90, 90);
    ratioLabel.setBounds(knob->getX() + 15, knob->getY() + labelOffset, knob->getWidth()-30, 20);
    knobU->setBounds(paddingx, paddingy, 90, 90);
    ratioLabel.setBounds(knobU->getX() + 15, knobU->getY() + labelOffset, knobU->getWidth() - 30, 20);

    knob1->setBounds(paddingx*2, paddingy, 90, 90);
    attacklabel.setBounds(knob1->getX() + 15, knob1->getY() + labelOffset , knob1->getWidth() - 30, 20);
    
    knob2->setBounds(paddingx*3 - 10, paddingy - 10, 90, 90);
    releaselabel.setBounds(knob2->getX() + 15, knob2->getY() + labelOffset, knob2->getWidth() - 30, 20);
    
    
    charCurve.setBounds(152,391,410,248);
    
    Switch.setBounds(40, 240, 90, 90);
    
    knob4->setBounds(Switch.getX(), Switch.getY() + 180, 90, 90);
    makeuplabel.setBounds(knob4->getX() + 15, knob4->getY() + labelOffset + 5, knob4->getWidth() - 30, 20);

    knob3->setBounds(Switch.getX() , Switch.getY() + 315, 90, 90);
    thresholdlabel.setBounds(knob3->getX() + 15, knob3->getY() + labelOffset + 5, knob3->getWidth() - 30, 20);

    MeterL.setBounds(paddingMeter * 4, 389 , 15, 248);
    MeterR.setBounds(paddingMeter * 4 + 20 , 389 , 15, 248);
    //MeterRedu.setBounds(paddingx * 4 + 40, 391, 15, 200);


}

void ExpanderAudioProcessorEditor::timerCallback()
{
    charCurve.repaint();
}

void ExpanderAudioProcessorEditor::setlabel(juce::Label& Label, juce::String text)
{
    Label.setText(text, juce::dontSendNotification);
    Label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::whitesmoke);
    Label.setFont(15.0f);
    Label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(Label);
}

void ExpanderAudioProcessorEditor::Logaritmic(std::vector<float>& vector, float start, float stop, int numvalues)
{
    float num_decades = std::log10(stop / start);
    float factor = std::pow(10.0f, num_decades / (numvalues - 1));

    vector.resize(numvalues);
    vector[0] = start;
    for (int i = 1; i < numvalues; i++)
    {
        vector[i] = vector[i - 1] * factor;
    }
   
}

void ExpanderAudioProcessorEditor::SetKnobDecibels(std::vector<float>& vector, float start, float mid, float end)
{
    int size = 60;
    vector.resize(size);
    float largeStep = (mid - start) / (size / 2);
    float smallStep = (end - mid) / (size / 2);

    float current = start;
    vector[0] = start;
    for (int i = 1; i < size; i++)
    {
        if (current < mid)
        {
            vector[i] = current + largeStep;
            current += largeStep;
        }
        else if (current < end)
        {
            vector[i] = current + smallStep;
            current += smallStep;
        }
        else
        {
            vector[i] = end;
        }
    }

}


