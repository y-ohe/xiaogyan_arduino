#include "Xiaogyan.hpp"
#include <Arduino.h>

const uint8_t XiaogyanClass::TM1640_GRID_MAP[] =
{
    1,  3,  5,  6, 11, 12, 13, 14,
    0,  2,  4, 15, 10,  9,  8,  7,
};

const uint8_t XiaogyanClass::TM1640_ROW_MAP[] =
{
    6, 5, 7, 4, 0, 1, 2, 3,
};

const uint8_t XiaogyanClass::HT16K33_GRID_MAP[] =
{
    1,  3,  5,  7,  9, 11, 13, 15,
    0,  2,  4,  6,  8, 10, 12, 14,
};

const uint8_t XiaogyanClass::HT16K33_ROW_MAP[] =
{
    0, 1, 2, 3, 4, 5, 6, 7,
};

TM1640 XiaogyanClass::tm1640_{ TM1640_DIN_PIN, TM1640_SCLK_PIN, TM1640_GRID_MAP, TM1640_ROW_MAP };
HT16K33 XiaogyanClass::ht16k33_{ HT16K33_SDA_PIN, HT16K33_SCL_PIN, HT16K33_GRID_MAP, HT16K33_ROW_MAP };
LedMatrix XiaogyanClass::ledMatrix_{ ht16k33_ };

LedMatrixAGFX XiaogyanClass::ledMatrix{ ledMatrix_ };

Speaker XiaogyanClass::speaker{ SPEAKER_PIN, SPEAKER_ENABLE_PIN, SPEAKER_LEDC_CHANNEL };

Encoder XiaogyanClass::encoder{ ENCODER_A_PIN, ENCODER_B_PIN };

GpioOutputPin<XiaogyanClass::LED_PIN> XiaogyanClass::led;

void XiaogyanClass::begin()
{
    // Led Matrix
    XiaogyanClass::ht16k33_.begin();
    XiaogyanClass::ledMatrix_.begin(true, LedMatrix::ScreenRotation::Degree270);

    // Speaker
    XiaogyanClass::speaker.begin();
    XiaogyanClass::speaker.setPowerSupply(true);

    // Encoder
    encoder.begin();

    // LED
    led.begin();

    // Buttons
    buttonA.begin(INPUT_PULLUP);
    buttonB.begin(INPUT_PULLUP);
}

void XiaogyanClass::doWork()
{
    encoder.doWork();
}

XiaogyanClass Xiaogyan;
