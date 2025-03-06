#pragma once
#include <vector>

class WavetableOscillator
{
public:
	WavetableOscillator(std::vector<float> waveTable, double sampleRate);
	WavetableOscillator(const WavetableOscillator&) = delete;
	WavetableOscillator& operator=(const WavetableOscillator&) = delete;
	WavetableOscillator(WavetableOscillator&&) = default;
	WavetableOscillator& operator=(WavetableOscillator&&) = default;

	void setFrequency(float frequency);
	float getSample();
	void stop();
	bool isPlaying();

private:
	float linearInterpolation();

	std::vector<float> waveTable;
	double sampleRate;
	float index = 0.f;
	float indexIncrement = 0.f;
};

