#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
#include "WavetableOscillator.h"
#include "Envelope.h"

class MicroworldsSynth
{
public:
	void prepareToPlay(double sampleRate);
	void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

private:
	void initOscillators();
	static std::vector<float> generateSineWavetable();
	void handleMidiEvent(const juce::MidiMessage& midiEvent);
	static float midiNoteNumberToFrequency(int midiNoteNumber);
	void render(juce::AudioBuffer<float>& buffer, int startSample, int endSample);
	int stoppedNote = 0;
	
	double sampleRate;
	std::vector<WavetableOscillator> oscillators;
	Envelope envelope;
};