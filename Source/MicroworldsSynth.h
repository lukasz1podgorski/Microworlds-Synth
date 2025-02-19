#pragma once

#include <juce_audio_processors/juce_audio_processors.h>


class MicroworldsSynth
{
public:
	void prepareToPlay(double sampleRate);
	void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

private:
	void handleMidiEvent(const juce::MidiMessage& midiEvent);
	double sampleRate;
};