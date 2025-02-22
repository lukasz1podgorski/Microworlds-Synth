#include "MicroworldsSynth.h"

std::vector<float> MicroworldsSynth::generateSineWavetable()
{
	//wavetable length
	constexpr auto WAVETABLE_LENGTH = 64;
	const auto PI = 3.14159265358979323846f;
	std::vector<float> sineWavetable(WAVETABLE_LENGTH);

	for (auto i = 0; i < WAVETABLE_LENGTH; ++i)
	{
		sineWavetable[i] = std::sinf(2 * PI * static_cast<float>(i) /
			WAVETABLE_LENGTH);
	}

	return sineWavetable;
}

void MicroworldsSynth::initOscillators()
{
	oscillators.clear();
	constexpr auto OSCILLATORS_COUNT = 128;
	const auto sineWaveTable = generateSineWavetable();

	for (auto i = 0; i < OSCILLATORS_COUNT; ++i)
	{
		//creating an object directly in the vector
		oscillators.emplace_back(sineWaveTable, sampleRate);
	}
}

void MicroworldsSynth::prepareToPlay(double sampleRate)
{
	this->sampleRate = sampleRate;
	initOscillators();
}

void MicroworldsSynth::processBlock(juce::AudioBuffer<float>& buffer,
									juce::MidiBuffer& midiMessages)
{
	auto currentSample = 0;

	for (const auto midiMetadata : midiMessages)
	{
		const auto message = midiMetadata.getMessage();
		const auto messagePosition = static_cast<int>(message.getTimeStamp());

		render(buffer, currentSample, messagePosition);
		currentSample = messagePosition;
		handleMidiEvent(message);
	}

	render(buffer, currentSample, buffer.getNumSamples());

}

void MicroworldsSynth::render(juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
	auto* firstChannel = buffer.getWritePointer(0);

	for (auto& oscillator : oscillators)
	{
		if (oscillator.isPlaying())
		{
			for (auto sample = startSample; sample < endSample; ++sample)
			{
				firstChannel[sample] += oscillator.getSample();
			}
		}
	}

	for (auto channel = 1; channel < buffer.getNumChannels(); ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);
		std::copy(firstChannel + startSample, firstChannel + endSample,
			channelData + startSample);
	}
}

void MicroworldsSynth::handleMidiEvent(const juce::MidiMessage& midiMessage)
{
	if (midiMessage.isNoteOn())
	{
		const auto oscillatorID = midiMessage.getNoteNumber();
		const auto frequency = midiNoteNumberToFrequency(oscillatorID);
		oscillators[oscillatorID].setFrequency(frequency);

	}
	else if (midiMessage.isNoteOff())
	{
		const auto oscillatorID = midiMessage.getNoteNumber();
		oscillators[oscillatorID].stop();
	}
	else if (midiMessage.isAllNotesOff())
	{
		for (auto& oscillator : oscillators)
		{
			oscillator.stop();
		}
	}
}

float MicroworldsSynth::midiNoteNumberToFrequency(const int midiNoteNumber)
{
	constexpr auto A4_FREQUENCY = 440.f;
	constexpr auto A4_NOTE_NUMBER = 69.f;
	constexpr auto NOTES_IN_AN_OCTAVE = 12.f;
	return A4_FREQUENCY * std::powf(2.f, (static_cast<float>(midiNoteNumber) - 
		A4_NOTE_NUMBER) / NOTES_IN_AN_OCTAVE);
}