/*
  ==============================================================================

    Redution.cpp
    Created: 5 May 2023 11:28:37pm
    Author:  JUAN CAMILO

  ==============================================================================
*/

#include "Redution.h"

void Redution::paint(juce::Graphics& g)
{
    auto Bounds = getLocalBounds().toFloat();
    g.setColour(juce::Colours::black);
    g.fillRect(Bounds);

    g.setColour(juce::Colours::peru);
    const auto scaledY = juce::jmap(level, -60.f, 6.f, 0.f, static_cast<float> (getHeight()));
    g.fillRect(Bounds.removeFromBottom(scaledY));



}

void Redution::setLevel(const float value)
{
    level = value;
}
