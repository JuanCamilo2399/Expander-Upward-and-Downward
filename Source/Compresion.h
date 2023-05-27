/*
  ==============================================================================

    Compresion.h
    Created: 29 Apr 2023 2:17:38pm
    Author:  JUAN CAMILO

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <xkeycheck.h>

class Compresion
{
public:
    Compresion()
    {
        attack = 10e-3;
        release = 10e-3;
        threshold = -10;
        samplerate = 44100;
        
        yn1 = 0;
        yn11 = 0;
        ratio = 2;
        ratio2 = 0.2;
        redutionE = -96;
        redutionU = -96;
    }

    void setSamplerate(float newSamplerate);
    void setattack(float newattack);
    void setrelease(float newrelease);
    void settreshold(float newthreshold);
    void setratio(float newratio, juce::String ID);
    float procesSampleD(float sample);
    float processSampleU(float sample);
    float getRedution(juce::String ID);
    
    
private:
    float attack, release, ratio, ratio2, samplerate, yn1,yn11, threshold, redutionE, redutionU;
   
    

};