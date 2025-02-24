#pragma once
class Envelope
{
public:
    float adsr(float input, int trigger);
    void setAttack(float attackTime);
    void setDecay(float decayTime);
    void setSustain(float sustainLevel);
    void setRelease(float releaseTime);
    int trigger = 0;

private:
    double sampleRate;
    float input, output, amplitude,
        attack, decay, sustain, release;
    long holdTime = 1;
    long holdCount;
    int attackPhase, decayPhase, sustainPhase, holdPhase, releasePhase;
};