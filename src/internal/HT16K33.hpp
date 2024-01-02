/*
 * HT16K33.hpp
 * Copyright (C) 2024 Y.Ohe
 * MIT License
 */

#pragma once

#include <cstdint>

class HT16K33
{
private:
    static constexpr uint8_t I2C_ADDRESS = 0x70;

    static constexpr uint8_t REG_DISP_DATA_ADR_PT = 0x00;
    static constexpr uint8_t REG_SYSTEM_SETUP     = 0x20;
    static constexpr uint8_t HT16K33_OSC_ON       = 0x01;
    static constexpr uint8_t HT16K33_OSC_OFF      = 0x00;
    static constexpr uint8_t REG_DISPLAY_SETUP    = 0x80;
    static constexpr uint8_t HT16K33_DISP_ON      = 0x01;
    static constexpr uint8_t HT16K33_DISP_OFF     = 0x00;
    static constexpr uint8_t HT16K33_BLINK_OFF    = 0x00;
    static constexpr uint8_t HT16K33_BLINK_2HZ    = 0x02;
    static constexpr uint8_t HT16K33_BLINK_1HZ    = 0x04;
    static constexpr uint8_t HT16K33_BLINK_HALFHZ = 0x06;
    static constexpr uint8_t REG_DIMMING_SET      = 0xE0;

    int sdaPin_;
    int sclPin_;
    const uint8_t* gridMap_;
    const uint8_t* segMap_;

private:
    void init();
    void start();
    void end();

public:
    HT16K33(int sdaPin, int sclPin);
    HT16K33(int sdaPin, int sclPin, const uint8_t gridMap[16], const uint8_t segMap[8]);
    void begin();
    void setBrightness(int brightness);
    void setGrid(uint8_t grid, uint8_t segs);
    void setDisplayState(uint8_t state);
    void blinkRate(uint8_t blink);

};
