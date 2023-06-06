/*
 * Speaker2.cpp
 * Copyright (C) 2023 MATSUOKA Takashi <matsujirushi@live.jp>
 * MIT License
 */

////////////////////////////////////////////////////////////////////////////////
// Includes

#include "Speaker2.hpp"
#include <Arduino.h>

////////////////////////////////////////////////////////////////////////////////
// Speaker2

Speaker2* Speaker2::instance_ = nullptr;

void Speaker2::timerIsr()
{
    if (instance_->wavePtr_ != instance_->waveEnd_)
    {
        ledcWrite(instance_->ledControlChannel_, *instance_->wavePtr_++);
    }
    else
    {
        timerAlarmDisable(instance_->timer_);
    }
}

Speaker2::Speaker2(int speakerPin, int speakerEnablePin, int ledControlChannel) :
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

void Speaker2::begin()
{
    ledcAttachPin(speakerPin_, ledControlChannel_);
    const uint32_t frequency = ledcSetup(ledControlChannel_, SPEAKER_PWM_FREQUENCY, SPEAKER_PWM_RESOLUTION_BITS);
    if (frequency == 0) abort();

    timer_ = timerBegin(0, getApbFrequency() / SPEAKER_TIMER_FREQUENCY, true);
    timerAttachInterrupt(timer_, timerIsr, true);
}

void Speaker2::play(const uint8_t* wave, size_t length, uint32_t frequency)
{
    wavePtr_ = wave;
    waveEnd_ = wave + length;

    timerAlarmWrite(timer_, SPEAKER_TIMER_FREQUENCY / frequency, true);
    timerAlarmEnable(timer_);
}

////////////////////////////////////////////////////////////////////////////////
