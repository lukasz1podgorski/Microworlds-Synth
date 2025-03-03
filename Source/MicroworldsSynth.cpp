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
	envelope.setAttack(2000.f, sampleRate);
	envelope.setDecay(50.f, sampleRate);
	envelope.setSustain(0.3f);
	envelope.setRelease(2000.f, sampleRate);

	for (const auto midiMetadata : midiMessages)
	{
		const auto message = midiMetadata.getMessage();
		const auto messagePosition = static_cast<int>(message.getTimeStamp());
		
		render(buffer, currentSample, messagePosition);
		currentSample = messagePosition;
		handleMidiEvent(message);
		std::cout << "noteon: " << message.isNoteOn() << std::endl;
		std::cout << "TRIGGER: " << envelope.trigger << std::endl;
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
				//firstChannel[sample] = envelope.adsr(firstChannel[sample], envelope.trigger);
				//firstChannel[sample] += envelope.adsr(oscillator.getSample(), envelope.trigger);
				//firstChannel[sample] += oscillator.getSample();
				//firstChannel[sample] += envelope.adsr(oscillator.getSample(), envelope.trigger);
				//firstChannel[sample] = envelope.adsr(firstChannel[sample] + oscillator.getSample(), envelope.trigger);
				firstChannel[sample] = envelope.adsr(firstChannel[sample] + oscillator.getSample(), envelope.trigger);
				std::cout << "channeldata: " << firstChannel[sample] << std::endl;
				std::cout << "OscData: " << oscillator.getSample() << std::endl;

				//if (firstChannel[sample] == 0)
				//{
				//	envelope.trigger = 0;
				//}
			}
		}
	}

	for (int channel = 1; channel < buffer.getNumChannels(); ++channel)
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
		envelope.trigger = 1;
		std::cout << "envelope active key on: " << envelope.ADSRisActive() << std::endl;
		std::cout << "key pressed: " << envelope.trigger << std::endl;
		const auto oscillatorID = midiMessage.getNoteNumber();
		const auto frequency = midiNoteNumberToFrequency(oscillatorID);
		oscillators[oscillatorID].setFrequency(frequency);
	
	}
	else if (midiMessage.isNoteOff())
	{
		envelope.trigger = 0;
		std::cout << "key off: " << envelope.trigger << std::endl;

		std::cout << "envelope not active key off: " << envelope.ADSRisActive() << std::endl;
		const auto oscillatorID = midiMessage.getNoteNumber();
		oscillators[oscillatorID].stop();

		//if (envelope.ADSRisActive() == false)
		//{
		//	std::cout << "envelope not active key off: " << envelope.ADSRisActive() << std::endl;
		//	const auto oscillatorID = midiMessage.getNoteNumber();
		//	oscillators[oscillatorID].stop();
		//}
		//else
		//{

		//}

	}
	else if (midiMessage.isAllNotesOff())
	{
		envelope.trigger = 0;

		for (auto& oscillator : oscillators)
		{
			if (envelope.ADSRisActive() == false)
			{
				oscillator.stop();
			}
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