#pragma once

#include "Arduino.h"
#include <board.h>

class ESP32_3248S035R : public Board {
public:
    void init() override;
    void afterLightSleep() override;
};
