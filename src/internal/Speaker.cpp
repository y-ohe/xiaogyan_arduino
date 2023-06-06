/*
 * Speaker.cpp
 * Copyright (C) 2023 MATSUOKA Takashi <matsujirushi@live.jp>
 * MIT License
 */

////////////////////////////////////////////////////////////////////////////////
// Includes

#include "Speaker.hpp"
#include <Arduino.h>

////////////////////////////////////////////////////////////////////////////////
// Speaker

Speaker* Speaker::instance_ = nullptr;

void Speaker::timerIsr()
{
    if (instance_ == nullptr) abort();

    if (instance_->wavePtr_ != instance_->waveEnd_)
    {
        ledcWrite(instance_->ledControlChannel_, *instance_->wavePtr_++);
    }
    else
    {
        instance_->stop();
    }
}

Speaker::Speaker(int speakerPin, int speakerEnablePin, int ledControlChannel) :
    speakerPin_{ speakerPin },
    speakerEnablePin_{ speakerEnablePin },
    ledControlChannel_{ ledControlChannel },
    timer_{ nullptr },
    wavePtr_{ nullptr },
    waveEnd_{ nullptr }
{
    if (instance_ != nullptr) abort();
    instance_ = this;
}

void Speaker::begin()
{
    if (speakerEnablePin_ >= 0)
    {
        pinMode(speakerEnablePin_, OUTPUT);
        digitalWrite(speakerEnablePin_, HIGH);
    }

    ledcAttachPin(speakerPin_, ledControlChannel_);
}

void Speaker::setPowerSupply(bool on)
{
    if (speakerEnablePin_ < 0) return;

    digitalWrite(speakerEnablePin_, on ? LOW : HIGH);
}

void Speaker::setTone(int frequency)
{
    ledcWriteTone(ledControlChannel_, frequency);
}

void Speaker::play(const uint8_t* wave, size_t length, uint32_t frequency)
{
    stop();

    const uint32_t pwmFrequency = ledcSetup(ledControlChannel_, SPEAKER_PWM_FREQUENCY, SPEAKER_PWM_RESOLUTION_BITS);
    if (pwmFrequency == 0) abort();

    wavePtr_ = wave;
    waveEnd_ = wave + length;

    timer_ = timerBegin(0, getApbFrequency() / SPEAKER_TIMER_FREQUENCY, true);
    if (timer_ == nullptr) abort();
    timerAttachInterrupt(timer_, timerIsr, true);
    timerAlarmWrite(timer_, SPEAKER_TIMER_FREQUENCY / frequency, true);
    timerAlarmEnable(timer_);
}

void Speaker::stop()
{
    if (timer_ != nullptr)
    {
        timerAlarmDisable(timer_);
        timerDetachInterrupt(timer_);
        timerEnd(timer_);
        timer_ = nullptr;
    }

    ledcWrite(ledControlChannel_, 0);
}

////////////////////////////////////////////////////////////////////////////////
