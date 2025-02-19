#include "MicroworldsSynth.h"

void MicroworldsSynth::prepareToPlay(double sampleRate)
{
	this->sampleRate = sampleRate;
}

void MicroworldsSynth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	auto currentSample = 0;

	for (const auto midiMessage : midiMessages)
	{
		const auto midiEvent = midiMessage.getMessage();
		const auto midiEventSample = static_cast<int>(midiEvent.getTimeStamp());

		//render(buffer, currentSample, midiEventSample);
		handleMidiEvent(midiEvent);

		currentSample = midiEventSample;
	}

	//render(buffer, currentSample, buffer.getNumSamples());

}

void MicroworldsSynth::handleMidiEvent(const juce::MidiMessage& midiEvent)
{
	if (midiEvent.isNoteOn())
	{
		const auto oscillatorID = midiEvent.getNoteNumber();
		//const auto frequency;

	}
	else if (midiEvent.isNoteOff())
	{

	}
	else if (midiEvent.isAllNotesOff())
	{

	}
}