#include "Envelope.h"
#include <juce_audio_processors/juce_audio_processors.h>

float Envelope::adsr(float input, int trigger)
{
	if (trigger = 1 && attackPhase != 1 && decayPhase != 1 && sustainPhase !=1)
	{
		attackPhase = 1;
		decayPhase = 0;
		sustainPhase = 0;
		releasePhase = 0;
	}
	
	if (attackPhase == 1)
	{
		releasePhase = 0;
		amplitude += (1 * attack);
		output = input * amplitude;
		
		if (amplitude >= 1)
		{
			amplitude = 1;
			attackPhase = 0;
			decayPhase = 1;
		}

	}

	if (decayPhase == 1)
	{
		output = input * (amplitude *= decay);

		if (amplitude <= sustain)
		{
			decayPhase = 0;
			sustain = amplitude;
			sustainPhase = 1;
		}
	}

	if (trigger == 1 && sustainPhase == 1)
	{
		output = input * amplitude;
	}

	if (trigger != 1 && sustainPhase == 1)
	{
		sustainPhase = 0;
		releasePhase = 1;
	}

	if (trigger != 1 && releasePhase == 1)
		output = input * (amplitude *= release);

	return output;
}

void Envelope::setAttack(float attackTime)
{
	attack = 1 - pow(0.01, 1.0 / (attackTime * sampleRate * 0.001));
}

void Envelope::setDecay(float decayTime)
{
	decay = pow(0.01, 1.0 / (decayTime * sampleRate * 0.001));
}

void Envelope::setSustain(float sustainLevel)
{
	sustain = sustainLevel;
}

void Envelope::setRelease(float releaseTime)
{
	release = pow(0.01, 1.0 / (releaseTime * sampleRate * 0.001));
}