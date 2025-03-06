#pragma once
class Envelope
{
public:
    float adsr(float input, int trigger);
    void setAttack(float attackTime, double sampleRate);
    void setDecay(float decayTime, double sampleRate);
    void setSustain(float sustainLevel);
    void setRelease(float releaseTime, double sampleRate);
    bool ADSRnoteOn();
    bool ADSRnoteOff();
    bool ADSRisActive();
    int trigger = 0;
    float amplitude = 0.f;
    bool ADSRoff = false;

private:
    float input, output, 
        //amplitude = 0.f,
        attack, decay, sustain, release, releaseStep;
    long holdTime = 1;
    long holdCount;
    int attackPhase, decayPhase, sustainPhase, holdPhase, releasePhase;
};