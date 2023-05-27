/*
  ==============================================================================

    Curve.h
    Created: 2 May 2023 6:53:15pm
    Author:  Wateo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Compresion.h"
#include <vector>

//==============================================================================
/*
*/
class Curve  : public juce::Component
{
public:
    Curve()
    {
        RMSValue = -100.f;
        inputValues.resize(5e3, 0);
        outputValues.resize(5e3, 0);

        inputValuesU.resize(5e3, 0);
        outputValuesU.resize(5e3, 0);

        for (int i = 0; i < inputValues.size(); i++)
        {
            inputValues[i] = (float(i) / inputValues.size());
        }

        for (int i = 0; i < inputValues.size(); i++)
        {
            inputValuesU[i] = (float(i) / inputValues.size());
        }

        comp.setSamplerate(inputValues.size() / 8);
        
        state = false;
    }

    ~Curve() override
    {

    }

    void paint (juce::Graphics& g) override
    {
        if (state == false)
        {
            juce::Path newPath;
            auto area = getLocalBounds();
            float width = area.getWidth();
            float height = area.getHeight();

            newPath.startNewSubPath(0, height);

            for (int i = 0; i < outputValues.size(); i++)
            {
                outputValues[i] = comp.procesSampleD(inputValues[i]);
            }

            for (int i = 0; i < getWidth(); i++)
            {
                int outPos = round(i * (outputValues.size() / width));

                float yvalue = outputValues[outPos];

                yvalue = yvalue > 0 ? 20 * log10(yvalue) : -40;

                yvalue = (yvalue / -40) * height;


                float xvalue = inputValues[outPos];

                xvalue = xvalue > 0 ? 20 * log10(xvalue) : -40;

                xvalue = (1 - (xvalue / -40)) * width;

                xvalue = xvalue < 0.0001 ? 0.0001 : xvalue;


                newPath.lineTo(xvalue, yvalue);
            }

            g.setColour(juce::Colour(juce::Colours::peru));
            g.strokePath(newPath, juce::PathStrokeType(3.0f, juce::PathStrokeType::JointStyle::mitered));
            newPath.closeSubPath();

            juce::Path newPath2;
            newPath.startNewSubPath(0, height);
            for (int i = 0; i < getWidth(); i++)
            {
                int outPos = round(i * (outputValues.size() / width));
                float yvalue = (1 - inputValues[outPos]) * height;
                float xvalue = inputValues[outPos] * width;

                newPath2.lineTo(xvalue, yvalue);
            }
            g.setColour(juce::Colour(juce::Colour(juce::uint8(127), juce::uint8(127), juce::uint8(127), 0.7f)));
            g.strokePath(newPath2, juce::PathStrokeType(1.0f, juce::PathStrokeType::curved));

            g.setColour(juce::Colours::red);
            float A = ((1 - (RMSValue / -100)) * width);
            float B = ((RMSValue / -100) * height);
            g.fillEllipse(A - 5, B - 5, 10, 10);
        }
        else
        {
            juce::Path newPathU;
            auto area = getLocalBounds();
            float width = area.getWidth();
            float height = area.getHeight();

            newPathU.startNewSubPath(0, height);

            for (int i = 0; i < outputValues.size(); i++)
            {
                outputValuesU[i] = comp.processSampleU(inputValuesU[i]);
            }

            for (int i = 0; i < getWidth(); i++)
            {
                int outPos = round(i * (outputValuesU.size() / width));

                float yvalue = outputValuesU[outPos];

                yvalue = yvalue > 0 ? 20 * log10(yvalue) : -40;

                yvalue = (yvalue / -40) * height;


                float xvalue = inputValuesU[outPos];

                xvalue = xvalue > 0 ? 20 * log10(xvalue) : -40;

                xvalue = (1 - (xvalue / -40)) * width;

                


                newPathU.lineTo(xvalue, yvalue);
            }

            g.setColour(juce::Colour(juce::Colours::peru));
            g.strokePath(newPathU, juce::PathStrokeType(3.0f, juce::PathStrokeType::JointStyle::mitered));
            newPathU.closeSubPath();

            juce::Path newPath2U;
            newPathU.startNewSubPath(0, height);
            for (int i = 0; i < getWidth(); i++)
            {
                int outPos = round(i * (outputValuesU.size() / width));
                float yvalue = (1 - inputValuesU[outPos]) * height;
                float xvalue = inputValuesU[outPos] * width;

                newPath2U.lineTo(xvalue, yvalue);
            }
            g.setColour(juce::Colour(juce::Colour(juce::uint8(127), juce::uint8(127), juce::uint8(127), 0.7f)));
            g.strokePath(newPath2U, juce::PathStrokeType(1.0f, juce::PathStrokeType::curved));

            g.setColour(juce::Colours::red);
            float A = ((1 - (RMSValue / -100)) * width);
            float B = ((RMSValue / -100) * height);
            g.fillEllipse(A - 5, B - 5, 10, 10);
        }


    }

    void resized() override
    {

    }

    void setRMSValue(float newRMS)
    {
        RMSValue = newRMS;
    }

    void setstate(bool newstate)
    {
        state = newstate;
    }


    Compresion comp;
private:
    std::vector<float> inputValues;
    std::vector<float> outputValues;
    std::vector<float> inputValuesU;
    std::vector<float> outputValuesU;
    std::atomic<float> RMSValue;
    bool state;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Curve)
};
