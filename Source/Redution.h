/*
  ==============================================================================

    Redution.h
    Created: 5 May 2023 11:28:37pm
    Author:  JUAN CAMILO

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Redution : public juce::Component
{
public:
    void paint(juce::Graphics& g) override;
    void setLevel(const float value);
private:
    float level = -60.f;
    
};