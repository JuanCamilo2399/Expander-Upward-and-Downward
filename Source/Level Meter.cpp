/*
  ==============================================================================

    Level Meter.cpp
    Created: 2 May 2023 12:15:03pm
    Author:  JUAN CAMILO

  ==============================================================================
*/

#include "Level Meter.h"

void HorizontalMeter::paint(juce::Graphics& g)
{
    if (state)
    {
        const auto level = valueSupplier();

        auto bounds = getLocalBounds().toFloat();

        g.setColour(juce::Colours::black);
        g.fillRect(bounds);



        g.setColour(juce::Colours::black);


        g.setGradientFill(gradient);
        const auto scaledY = juce::jmap(level, -60.f, 6.f, 0.f, static_cast<float> (getHeight()));
        g.fillRect(bounds.removeFromBottom(scaledY));
    }
    else
    {
        auto Bounds = getLocalBounds().toFloat();

        g.setColour(juce::Colours::black);
        g.fillRect(Bounds);

        g.setColour(juce::Colours::peru);
        const auto scaledY = juce::jmap(level2, -60.f, 6.f, 0.f, static_cast<float> (getHeight()));
        g.fillRect(Bounds.removeFromBottom(scaledY));
    }

}

void HorizontalMeter::resized()
{
    const auto bounds = getLocalBounds().toFloat();
    
        gradient = juce::ColourGradient
        {
            juce::Colours::green,
            bounds.getBottomLeft(),
            juce::Colours::red,
            bounds.getTopLeft(),
            false
        };
        gradient.addColour(0.5, juce::Colours::yellow);
    
    


}

void HorizontalMeter::setLevel(const float value)
{
    level2 = value;
}

void HorizontalMeter::timerCallback()
{
    repaint();
}
