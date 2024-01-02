/*
 * TM1640.cpp
 * Copyright (C) 2023 MATSUOKA Takashi <matsujirushi@live.jp>
 * MIT License
 */

////////////////////////////////////////////////////////////////////////////////
// Includes

#include "HT16K33.hpp"
#include <Arduino.h>
#include <Wire.h>

////////////////////////////////////////////////////////////////////////////////
// HT16K33

void HT16K33::init()
{
    Wire.begin();
    Wire.setClock(400000);

    // Internal system clock settup
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(REG_POWER_CTL | OSC_ON);
    if (Wire.endTransmission() != 0) abort();

    // ROW/INT output pin set

    // Dimming set
    setBrightness(15);

    // Blinking set
    setDisplayState(HT16K33_DISP_ON);
    blinkRate(HT16K33_BLINK_OFF);
}

void HT16K33::start()
{
}

void HT16K33::end()
{
}

HT16K33::HT16K33(int sdaPin, int sclPin) :
    sdaPin_{ sdaPin },
    sclPin_{ sclPin },
    gridMap_{ nullptr },
    segMap_{ nullptr }
{
}

HT16K33::HT16K33(int sdaPin, int sclPin, const uint8_t gridMap[16], const uint8_t segMap[8]) :
    sdaPin_{ sdaPin },
    sclPin_{ sclPin },
    gridMap_{ gridMap },
    segMap_{ segMap }
{
}

void HT16K33::begin()
{
    init();

    start();
    write(0b01000000);  // Data command setting : Address auto + 1
    end();
}

void HT16K33::setBrightness(int brightness)
{
    if (brightness < 0)  brightness = 0;
    if (brightness > 15) brightness = 15; // limit to max brightness
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(REG_DIMMING_SET | brightnes);
    if (Wire.endTransmission() != 0) abort();
}

void HT16K33::setDisplayState(bool state)
{
    Wire.beginTransmission(I2C_ADDRESS);
    if(state) Wire.write(REG_DISPLAY_SETUP | HT16K33_DISP_ON);
    else Wire.write(REG_DISPLAY_SETUP | HT16K33_DISP_OFF);
    if (Wire.endTransmission() != 0) abort();
}

void HT16K33::blinkRate(uint8_t blink)
{
    if(blink / 2 > 3) blink = 0;
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(REG_DISPLAY_SETUP | HT16K33_DISP_ON | blink);
    if (Wire.endTransmission() != 0) abort();
}

void HT16K33::setGrid(uint8_t grid, uint8_t segs)
{
    if (grid >= 16) return;

    if (gridMap_ != nullptr)
    {
        grid = gridMap_[grid];
        if (grid >= 16) return;
    }

    if (segMap_ != nullptr)
    {
        uint8_t actualSegs = 0;
        for (int i = 0; i < 8; ++i) if (segs & 1 << i) actualSegs |= 1 << segMap_[i];
        segs = actualSegs;
    }

    start();
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(REG_DISP_DATA_ADR_PT | grid);
    Wire.write(segs);
    if (Wire.endTransmission() != 0) abort();
    end();
}

////////////////////////////////////////////////////////////////////////////////
