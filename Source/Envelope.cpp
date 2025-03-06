#include "Envelope.h"
#include <juce_audio_processors/juce_audio_processors.h>

float Envelope::adsr(float input, int trigger)
{
	if (trigger == 1 && attackPhase != 1 && decayPhase != 1 && sustainPhase != 1)
	{
		attackPhase = 1;
		std::cout << "attack phase triggered: " << attackPhase << std::endl;
		decayPhase = 0;
		sustainPhase = 0;
		releasePhase = 0;
		ADSRoff = false;
	}
	
	if (attackPhase == 1)
	{
		releasePhase = 0;
		amplitude += (1 * attack);
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
		std::cout << "release phase triggered: " << releasePhase << std::endl;
	}

	if (releasePhase == 1 && amplitude > 0.f)
	{

		if (amplitude > 0.0001)
		{
			output = input * (amplitude *= release);
		}
		else
		{
			amplitude -= releaseStep;
			if (amplitude < 0.0) amplitude = 0.0;
			//amplitude = 0.f;
		}

		if (amplitude == 0.f)
		{
			releasePhase = 0;
			output = 0.f;
			ADSRoff = true;
			std::cout << "ADSR off MUSI byæ 1: " << ADSRoff << std::endl;
			std::cout << "release phase code FINISHED: " << true << std::endl;
		}
		//else if (amplitude != 0.f && trigger == 1)
		//{
		//	releasePhase = 0;
		//	ADSRoff = true;
		//	attackPhase = 1;
		//}

		//if (trigger == 1)
		//{
		//	attackPhase = 1;
		//}
	}
	
	return output;
}

void Envelope::setAttack(float attackTime, double sampleRate)
{
	attack = 1 - pow(0.01, 1.0 / (attackTime * sampleRate * 0.001));
}

void Envelope::setDecay(float decayTime, double sampleRate)
{
	decay = pow(0.01, 1.0 / (decayTime * sampleRate * 0.001));
}

void Envelope::setSustain(float sustainLevel)
{
	sustain = sustainLevel;
}

void Envelope::setRelease(float releaseTime, double sampleRate)
{
	//release = pow(0.01, 1.0 / (releaseTime * sampleRate * 0.001));
	release = exp(-5.0 / (releaseTime * sampleRate * 0.001));
	releaseStep = amplitude / (sampleRate * 0.005);
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