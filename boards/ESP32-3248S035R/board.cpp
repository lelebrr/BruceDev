#include "board.h"
#include <display.h>
#include <button.h>

void ESP32_3248S035R::init() {
    // Buttons
    button.setLongPressTime(500);

    // Display
    display.init();
    display.setRotation(ROTATION);
    display.setBrightness(DEFAULT_BRIGHTNESS);
}

void ESP32_3248S035R::afterLightSleep() {
    // Display
    display.init();
}
