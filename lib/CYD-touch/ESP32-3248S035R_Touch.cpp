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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
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
 * 08.2023 - Piotr Zapart www.hexefx.com
 *
 */
#include "ESP32-3248S035R_Touch.h"

#define ESP32_3248S035R_Touch_SPI_SETTINGS SPISettings(2500000, MSBFIRST, SPI_MODE0)

#if defined(ESP32)

static void IRAM_ATTR __on_touch_irq_changed()
{
  // a IRAM function to wake up the main task
  // more on: https://www.xtronical.com/esp32/esp32-external-interrupts-part-1-a-brief-tutorial/
  // and: https://www.xtronical.com/esp32/esp32-external-interrupts-part-2-give-and-take-a-semaphore/
//  touch_touched = true;
}
#endif

bool ESP32_3248S035R_Touch::begin(SPIClass *tspi) {

  _pspi = tspi;

  return true;
}

bool ESP32_3248S035R_Touch::begin()
{
  pinMode(ESP32_3248S035R_Touch_IRQ, INPUT_PULLUP);
  pinMode(ESP32_3248S035R_Touch_CS, OUTPUT);
  digitalWrite(ESP32_3248S035R_Touch_CS, HIGH);

  pinMode(ESP32_3248S035R_Touch_CLK, OUTPUT);
  pinMode(ESP32_3248S035R_Touch_MOSI, OUTPUT);
  pinMode(ESP32_3248S035R_Touch_MISO, INPUT);

  digitalWrite(ESP32_3248S035R_Touch_CLK, HIGH);
  digitalWrite(ESP32_3248S035R_Touch_MOSI, HIGH);

#if defined(ESP32)
  attachInterrupt(ESP32_3248S035R_Touch_IRQ, __on_touch_irq_changed, FALLING); //attatch interrupt to the pin on FALLING edge
#endif

  return true;
}
#define SWSPI_BEGIN_END _pspi->endTransaction(); \
                        _pspi->begin(); \
                        _pspi->beginTransaction(ESP32_3248S035R_Touch_SPI_SETTINGS)

#define SWSPI_XFER(val) _pspi->transfer(val);
uint8_t ESP32_3248S035R_Touch::transfer(uint8_t d)
{
    SWSPI_BEGIN_END;
    return SWSPI_XFER(d);
}

uint16_t ESP32_3248S035R_Touch::transfer16(uint16_t d)
{
    SWSPI_BEGIN_END;
    return _pspi->transfer16(d);

}

ESP32_3248S035R_TS_Point ESP32_3248S035R_Touch::getPointRaw()
{
  update();
  return ESP32_3248S035R_TS_Point(xraw, yraw, zraw);
}

ESP32_3248S035R_TS_Point ESP32_3248S035R_Touch::getPointScaled()
{
  update();
  int16_t x,y;
  x = xraw;
  y = yraw;
  convertRawXY(&x, &y);
  return ESP32_3248S035R_TS_Point(x,y,zraw);
}

void ESP32_3248S035R_Touch::wait(uint_fast8_t del)
{
  for(uint_fast8_t i=0; i < del; ++i)
  {
      __asm__("nop");
  }
}

void ESP32_3248S035R_Touch::update()
{
  int16_t data[6];

  if (_pspi == nullptr)
  {
    // bit-banged SPI
    digitalWrite(ESP32_3248S035R_Touch_CS, LOW);
    for(uint_fast8_t i=0; i<8; ++i)
    {
        digitalWrite(ESP32_3248S035R_Touch_CLK, LOW);
        wait(_delay);
        digitalWrite(ESP32_3248S035R_Touch_CLK, HIGH);
        wait(_delay);
    }

    //digitalWrite(ESP32_3248S035R_Touch_CS, LOW);
    transfer(0xB1 /* Z1 */);
    data[0] = transfer16(0xC1 /* Z2 */) >> 3;
    data[1] = transfer16(0x91 /* Y */) >> 3;
    data[2] = transfer16(0xD1 /* X */) >> 3;
    data[3] = transfer16(0xB1 /* Z1 */) >> 3;
    data[4] = transfer16(0xC1 /* Z2 */) >> 3;
    data[5] = transfer16(0x91 /* Y */) >> 3;
    transfer(0xD0 /* X */);
    transfer16(0);
    digitalWrite(ESP32_3248S035R_Touch_CS, HIGH);

  }
  else
  {
    // hardware SPI
    _pspi->beginTransaction(ESP32_3248S035R_Touch_SPI_SETTINGS);
    digitalWrite(ESP32_3248S035R_Touch_CS, LOW);
    _pspi->transfer(0xB1 /* Z1 */);
    data[0] = _pspi->transfer16(0xC1 /* Z2 */) >> 3;
    data[1] = _pspi->transfer16(0x91 /* Y */) >> 3;
    data[2] = _pspi->transfer16(0xD1 /* X */) >> 3;
    data[3] = _pspi->transfer16(0xB1 /* Z1 */) >> 3;
    data[4] = _pspi->transfer16(0xC1 /* Z2 */) >> 3;
    data[5] = _pspi->transfer16(0x91 /* Y */) >> 3;
    _pspi->transfer(0xD0 /* X */);
    _pspi->transfer16(0);
    digitalWrite(ESP32_3248S035R_Touch_CS, HIGH);
    _pspi->endTransaction();
  }
	//
	if ( (data[0] < threshold) || (data[3] < threshold) || (data[4] < threshold) ) zraw = 0;
  else zraw = 4095;

	if (zraw > threshold)
	{
		// average the 3 raw data points
		xraw = (data[2] + 4095-data[2])/2;
		yraw = (data[1] + data[5])/2;
	} else
	{
		xraw = yraw = zraw = 0;
	}
}

bool ESP32_3248S035R_Touch::touched()
{
  if (_pspi == nullptr)
  {
     return digitalRead(ESP32_3248S035R_Touch_IRQ) == LOW;
  }
   update();
   return (zraw >= threshold);
}


void ESP32_3248S035R_Touch::readData(uint16_t *x, uint16_t *y, uint8_t *z)
{
  update();
  *x = xraw;
  *y = yraw;
  *z = zraw;
}

void ESP32_3248S035R_Touch::convertRawXY(int16_t *x, int16_t *y)
{
	int32_t x_tmp, y_tmp;

  if (rotation == 1) // 90
  {
    y_tmp = ( (int32_t)(*x - CYD28_TouchR_CAL_XMIN) * (int32_t)sizeY_px) / (CYD28_TouchR_CAL_XMAX - CYD28_TouchR_CAL_XMIN);
    x_tmp = ( (int32_t)(*y - CYD28_TouchR_CAL_YMIN) * (int32_t)sizeX_px) / (CYD28_TouchR_CAL_YMAX - CYD28_TouchR_CAL_YMIN);
    x_tmp = sizeX_px - x_tmp;
  }
  else if (rotation == 2) // 180
  {
    x_tmp = ( (int32_t)(*x - CYD28_TouchR_CAL_XMIN) * (int32_t)sizeX_px) / (CYD28_TouchR_CAL_XMAX - CYD28_TouchR_CAL_XMIN);
    y_tmp = ( (int32_t)(*y - CYD28_TouchR_CAL_YMIN) * (int32_t)sizeY_px) / (CYD28_TouchR_CAL_YMAX - CYD28_TouchR_CAL_YMIN);
    x_tmp = sizeX_px - x_tmp;
    y_tmp = sizeY_px - y_tmp;
  }
  else if (rotation == 3) // 270
  {
    y_tmp = ( (int32_t)(*x - CYD28_TouchR_CAL_XMIN) * (int32_t)sizeY_px) / (CYD28_TouchR_CAL_XMAX - CYD28_TouchR_CAL_XMIN);
    x_tmp = ( (int32_t)(*y - CYD28_TouchR_CAL_YMIN) * (int32_t)sizeX_px) / (CYD28_TouchR_CAL_YMAX - CYD28_TouchR_CAL_YMIN);
    y_tmp = sizeY_px - y_tmp;
  }
  else // 0
  {
    x_tmp = ( (int32_t)(*x - CYD28_TouchR_CAL_XMIN) * (int32_t)sizeX_px) / (CYD28_TouchR_CAL_XMAX - CYD28_TouchR_CAL_XMIN);
    y_tmp = ( (int32_t)(*y - CYD28_TouchR_CAL_YMIN) * (int32_t)sizeY_px) / (CYD28_TouchR_CAL_YMAX - CYD28_TouchR_CAL_YMIN);
  }
    *x = x_tmp;
    *y = y_tmp;
}
