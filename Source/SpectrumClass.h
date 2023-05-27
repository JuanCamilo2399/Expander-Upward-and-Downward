/*
  ==============================================================================

    SpectrumClass.h
    Created: 17 Jun 2020 11:44:47am
    Author:  WATEO

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class SpectrumClass : public juce::Component
{
public:
	SpectrumClass(int order, int samples) :
		FFTProcessor(order), window(samples, juce::dsp::WindowingFunction<float>::WindowingMethod::hamming),
		bufferSize(samples)
	{
		data.reset (new float[samples * 2]);
		juce::zeromem(data.get(), sizeof(float) * samples * 2);
	}

	~SpectrumClass()
	{

	}

	void receiveAndProcess(float* buffer)
	{
		memcpy(data.get(), buffer, sizeof(float) * bufferSize);

		window.multiplyWithWindowingTable(data.get(), bufferSize);
		FFTProcessor.performFrequencyOnlyForwardTransform(data.get());

		juce::FloatVectorOperations::multiply (data.get(), (1 / (float)(bufferSize / 2)), bufferSize);



		repaint();
	}

	void paint(juce::Graphics& g) override
	{
		
		juce::Path magnitude;

		magnitude.startNewSubPath(0, getHeight());

		float min = -80;
		float max = 0;

		float step = getWidth() / (float)(bufferSize / 2);


		for (auto i = 0; i < floor(bufferSize / 2); ++i)
		{
			int posX = (log10(step * i) / log10(getWidth())) * (getWidth() - 1);

			posX = posX < 0 ? 0 : posX;
			
			float valueY = (1 - (juce::jlimit(min, max, juce::Decibels::gainToDecibels(data[i])) / min)) * getHeight();

			magnitude.lineTo(posX, getHeight() - valueY);
		}

		g.setColour(juce::Colour(juce::Colours::white));
		g.strokePath(magnitude, juce::PathStrokeType(1.0f, juce::PathStrokeType::curved));
	}

private:
	int bufferSize;
	std::unique_ptr<float[]> data;
	juce::dsp::FFT FFTProcessor;
	juce::dsp::WindowingFunction<float> window;
};
