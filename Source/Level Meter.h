/*
  ==============================================================================

    Level Meter.h
    Created: 2 May 2023 12:15:03pm
    Author:  JUAN CAMILO

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class HorizontalMeter : public juce::Component, public juce::Timer
{
public:
    HorizontalMeter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction))
    {
        startTimerHz(30);
        state = true;
    }
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    void setLevel(const float value);
    void timerCallback() override;

    bool state;

private:
    float level2 = -60.f;
    std::function<float()> valueSupplier;
    juce::ColourGradient gradient{};
    

};