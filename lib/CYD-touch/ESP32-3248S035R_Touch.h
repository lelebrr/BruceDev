/* Touchscreen library for XPT2046 Touch Controller Chip
 * Copyright (c) 2015, Paul Stoffregen, paul@pjrc.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ----------------------- ESP32-3248S035R (3.5" cheap yellow display) version --------------------------
 *   using software bit banged SPI and fixed GPIOs  + manual calibration
 *
 */

#ifndef _ESP32_3248S035R_TOUCH_H_
#define _ESP32_3248S035R_TOUCH_H_

#include <Arduino.h>
#include <SPI.h>

#define ESP32_3248S035R_Touch_Z_THRESH       300
#define ESP32_3248S035R_Touch_Z_THRES_INT  75

// These definitions come from https://github.com/rzeldent/platformio-espressif32-sunton board definitions
#if defined(TOUCH_XPT2046_SPI)
  #define ESP32_3248S035R_Touch_IRQ    XPT2046_TOUCH_CONFIG_INT_GPIO_NUM
  #define ESP32_3248S035R_Touch_MOSI   XPT2046_SPI_BUS_MOSI_IO_NUM
  #define ESP32_3248S035R_Touch_MISO   XPT2046_SPI_BUS_MISO_IO_NUM
  #define ESP32_3248S035R_Touch_CLK    XPT2046_SPI_BUS_SCLK_IO_NUM
  #define ESP32_3248S035R_Touch_CS     XPT2046_SPI_CONFIG_CS_GPIO_NUM
#else
  #define ESP32_3248S035R_Touch_IRQ  36
  #define ESP32_3248S035R_Touch_MOSI   32
  #define ESP32_3248S035R_Touch_MISO   39
  #define ESP32_3248S035R_Touch_CLK  25
  #define ESP32_3248S035R_Touch_CS   33

#endif
// CALIBRAION VALUES
#define ESP32_3248S035R_Touch_CAL_XMIN 185
#define ESP32_3248S035R_Touch_CAL_XMAX 3700
#define ESP32_3248S035R_Touch_CAL_YMIN 280
#define ESP32_3248S035R_Touch_CAL_YMAX 3850


class ESP32_3248S035R_TS_Point {
public:
  ESP32_3248S035R_TS_Point(void) : x(0), y(0), z(0) {}
  ESP32_3248S035R_TS_Point(int16_t x, int16_t y, int16_t z) : x(x), y(y), z(z) {}
  bool operator==(ESP32_3248S035R_TS_Point p) { return ((p.x == x) && (p.y == y) && (p.z == z)); }
  bool operator!=(ESP32_3248S035R_TS_Point p) { return ((p.x != x) || (p.y != y) || (p.z != z)); }
  int16_t x, y, z;
};

class ESP32_3248S035R_Touch {
public:
  constexpr ESP32_3248S035R_Touch(int32_t w, int32_t h)
    : _delay(2), sizeX_px(w), sizeY_px(h){ }
  bool begin();
  bool begin(SPIClass *tspi);

  ESP32_3248S035R_TS_Point getPointScaled();
  ESP32_3248S035R_TS_Point getPointRaw();
  bool touched();
  void readData(uint16_t *x, uint16_t *y, uint8_t *z);
  void setRotation(uint8_t n) { rotation = n % 4; }
    void setThreshold(uint16_t th) { threshold = th;}

  volatile bool isrWake=true;

private:
  void update();
  uint8_t transfer(uint8_t);
  uint16_t transfer16(uint16_t data);
  void wait(uint_fast8_t del);
  void convertRawXY(int16_t *x, int16_t *y);
  uint8_t rotation=1;
  int16_t xraw=0, yraw=0, zraw=0;
    uint16_t threshold = ESP32_3248S035R_Touch_Z_THRESH;
  uint32_t msraw=0x80000000;
  uint8_t _delay;
  const  int32_t sizeX_px;
  const int32_t sizeY_px;
  SPIClass *_pspi = nullptr;

};

#endif
