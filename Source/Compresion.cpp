/*
  ==============================================================================

    Compresion.cpp
    Created: 29 Apr 2023 2:17:38pm
    Author:  JUAN CAMILO

  ==============================================================================
*/

#include "Compresion.h"
#include <cmath>
void Compresion::setSamplerate(float newSamplerate)
{
    samplerate = newSamplerate;
}

void Compresion::setattack(float newattack)
{
    attack = newattack;
}

void Compresion::setrelease(float newrelease)
{
    release = newrelease;
}

void Compresion::settreshold(float newthreshold)
{
    threshold = newthreshold;
}

void Compresion::setratio(float newratio, juce::String ID)
{
    if (ID == "Down" || ID == "DOWN" )
    {
        ratio = newratio;
    }
    else if (ID == "Up" || ID == "UP")
    {
        ratio2 = newratio;
    }
}



float Compresion::procesSampleD(float sample)
{
    
    auto xn = std::abs(sample);
    if (xn < 0.00001)
    {
        xn = 0.00001;
    }
    auto xndb = 20 * std::log10(xn);
    float residuodB;
    float yout;

    float alphaA = std::expf(-1 / (attack * samplerate));
    float alphaR = std::expf(-1 / (release * samplerate));

 
    if (xndb < threshold)
    {
        if (ratio == 100)
        {
            residuodB = -96.0f;
        }
        else
        {
            residuodB = xndb + ((xndb - threshold) * (1 - (1/ratio)));
        }
        
    }
    else
    {
        residuodB = xndb;
    }
    auto xl = xndb - residuodB;

    
    

    if (xl < yn1)
    {
        yout = (alphaA * yn1) + ((1 - alphaA) * xl);
    }
    else
    {
        yout = (alphaR * yn1) + ((1 - alphaR) * xl);
    }

    yn1 = yout;
    redutionE = -yout;
    auto reduccion = std::powf(10, ((-1 * yout) / 20));

    auto Nsample = sample * reduccion;

    return Nsample;

}
float Compresion::processSampleU(float sample)
{
    auto xn = std::abs(sample);
    if (xn < 0.00001)
    {
        xn = 0.00001;
    }
    auto xndb = 20 * std::log10(xn);
    float residuodB;
    float yout;

    float alphaA = std::expf(-1 / (attack * samplerate));
    float alphaR = std::expf(-1 / (release * samplerate));
    if (xndb > threshold)
    {
        if (ratio2 == 1)
        {
            residuodB = xndb;
        }
        else
        {
            residuodB = xndb - ((threshold - xndb) * ratio2);
        }
         
    }
    else

    {
        residuodB = xndb;
    }

    auto xl = residuodB - xndb;

    
    

    if (xl > yn11)
    {
        yout = (alphaA * yn11) + ((1 - alphaA) * xl);
    }
    else
    {
        yout = (alphaR * yn11) + ((1 - alphaR) * xl);
    }

    yn11 = yout;
    redutionU = -yout;
    auto reduccion = std::powf(10, ((yout) / 20));

    auto Nsample = sample * reduccion;



    return Nsample;
    
}

float Compresion::getRedution(juce::String ID)
{
    if (ID == "Down" || ID == "DOWN")
    {
        return redutionE;
    }
    else if (ID == "Up" || ID == "UP")
    {
        return redutionU;
    }
}






