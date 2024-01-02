/*
 * LedMatrix.hpp
 * Copyright (C) 2023 MATSUOKA Takashi <matsujirushi@live.jp>
 * MIT License
 */

#pragma once

#include <cstdbool>
#include <cstdint>
#include <ArduinoEigen.h>

class HT16K33;

class LedMatrix
{
public:
    enum class ScreenRotation
    {
        Degree0,
        Degree90,
        Degree180,
        Degree270,
    };

private:
    static constexpr int WIDTH = 8;
    static constexpr int HEIGHT = 8;
    static constexpr int COLOR_BITS = 2;

private:
    HT16K33& ledDriver_;
    Eigen::Matrix<int, 3, 3> screenAffineTransformation_;
    uint8_t buffer_[COLOR_BITS][HEIGHT];

public:
    int width() const
    {
        return WIDTH;
    }

    int height() const
    {
        return HEIGHT;
    }

    HT16K33& getLedDriver() const
    {
        return ledDriver_;
    }

public:
    explicit LedMatrix(HT16K33& ledDriver);
    void begin();
    void begin(bool flip, ScreenRotation screenRotation);
    int getPixel(int x, int y) const;
    void setPixel(int x, int y, int color);

};
