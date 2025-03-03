#include "Envelope.h"
#include <juce_audio_processors/juce_audio_processors.h>

float Envelope::adsr(float input, int trigger)
{
	std::cout << "input: " << input << std::endl;
	if (trigger == 1 && attackPhase != 1 && decayPhase != 1 && sustainPhase != 1)
	{
		attackPhase = 1;
		std::cout << "attack phase triggered: " << attackPhase << std::endl;
		decayPhase = 0;
		sustainPhase = 0;
		releasePhase = 0;
	}
	
	if (attackPhase == 1)
	{
		releasePhase = 0;
		std::cout << "amplituda przed: " << amplitude << std::endl;
		amplitude += (1 * attack);
		std::cout << "amplituda po: " << amplitude << std::endl;
		std::cout << "attack in adsr " << attack << std::endl;
		output = input * amplitude;
		
		if (amplitude >= 1.0f)
		{
			amplitude = 1.0f;
			attackPhase = 0;
			decayPhase = 1;
			std::cout << "decay phase triggered: " << decayPhase << std::endl;
		}

	}

	if (decayPhase == 1)
	{
		output = input * (amplitude *= decay);

		if (amplitude <= sustain)
		{
			//amplitude = sustain;
			decayPhase = 0;
			sustainPhase = 1;
			std::cout << "sustain phase triggered: " << sustainPhase << std::endl;
			std::cout << "amplituda sustain: " << sustainPhase << std::endl;
		}
	}

	if (trigger == 1 && sustainPhase == 1)
	{
		output = input * amplitude;
	}

	if (trigger != 1 && sustainPhase == 1)
	{
		std::cout << "podniosl przyciks trigger musi byc 0: " << trigger << std::endl;
		sustainPhase = 0;
		releasePhase = 1;
		std::cout << "release phase triggered: " << releasePhase << std::endl;
	}

	if (releasePhase == 1 && amplitude > 0.f)
	{
		output = input * (amplitude *= release);

		if (amplitude == 0)
		{
			releasePhase = 0;
		}
	}
	
	std::cout << "output: " << output << std::endl;
	return output;
}

void Envelope::setAttack(float attackTime, double sampleRate)
{
	attack = 1 - pow(0.01, 1.0 / (attackTime * sampleRate * 0.001));
	std::cout << "attack in setAttack: " << attack << std::endl;
}

void Envelope::setDecay(float decayTime, double sampleRate)
{
	decay = pow(0.01, 1.0 / (decayTime * sampleRate * 0.001));
}

void Envelope::setSustain(float sustainLevel)
{
	sustain = sustainLevel;
	std::cout << sustain << std::endl;
}

void Envelope::setRelease(float releaseTime, double sampleRate)
{
	release = pow(0.01, 1.0 / (releaseTime * sampleRate * 0.001));
}

bool Envelope::ADSRnoteOn()
{
	if (trigger == 1 && (attackPhase == 1 || decayPhase == 1 || sustainPhase == 1))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Envelope::ADSRnoteOff()
{
	if (trigger != 1 || releasePhase == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Envelope::ADSRisActive()
{
	if ((attackPhase == 1) || (decayPhase == 1) || (sustainPhase == 1) || (releasePhase == 1))
	{
		return true;
	}
	else
	{
		return false;
	}
}