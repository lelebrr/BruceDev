#ifndef interface_h
#define interface_h

#include "Arduino.h"

// Funções principais de setup
void setupESP32_3248S035();
void initDisplay();
void initTouch();
void initSDCard();
void calibrateTouch();

// Funções de controle
void setBacklight(bool state);
void setRGBLed(int red, int green, int blue);
void setAudio(bool enabled);
void readLDR();

// Funções de informação
void displaySystemInfo();

// Funções de teste
void testDisplay();
void testTouch();
void testSDCard();
void testAudio();
void testRGBLed();
void testLDR();
void runHardwareTests();

// Variáveis globais externas
extern bool backlightState;
extern int ledRedValue;
extern int ledGreenValue;
extern int ledBlueValue;
extern float ldrValue;

// Constantes de pinos
#define BOOT_BUTTON 0
#define RESET_BUTTON -1
#define TFT_BACKLIGHT 27
#define LED_RED 17
#define LED_GREEN 16
#define LED_BLUE 2
#define LDR_SENSOR 4
#define AUDIO_SHUTDOWN 26
#define AUDIO_IN 25

// Constantes de display
#define TFT_WIDTH 320
#define TFT_HEIGHT 480
#define TFT_DRIVER ST7796

// Constantes de touch
#define TOUCH_WIDTH 320
#define TOUCH_HEIGHT 480
#define TOUCH_DRIVER XPT2046

// Constantes de SD Card
#define SDCARD_SPI_SPEED SPI_MODE0

#endif // interface_h
