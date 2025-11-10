#ifndef pins_arduino_h
#define pins_arduino_h

#define ESP32_3248S035

// ESP32 WROOM-32 Pin Definitions
#define EXTERNAL_NUM_INTERRUPTS 48
#define NUM_DIGITAL_PINS 48
#define NUM_ANALOG_INPUTS 16

// UART Pins
#define UART0_TXD 1
#define UART0_RXD 3
#define UART1_TXD 10
#define UART1_RXD 9
#define UART2_TXD 17
#define UART2_RXD 16

// SPI Pins (Display ST7796)
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS 15
#define TFT_DC 21
#define TFT_RST 4
#define TFT_BACKLIGHT 27

// Touch Resistive XPT2046 (SPI)
#define TOUCH_CS 13
#define TOUCH_CLK 14
#define TOUCH_DIN 12
#define TOUCH_DOUT 32
#define TOUCH_IRQ 33

// SD Card SPI (Compartilhado com Display)
#define SDCARD_CS 2
#define SDCARD_SCK 14
#define SDCARD_MISO 13
#define SDCARD_MOSI 15

// I2C Pins
#define I2C_SDA 22
#define I2C_SCL 23

// Audio Amplifier SC8002B
#define AUDIO_SHUTDOWN 26
#define AUDIO_IN 25

// LED RGB
#define LED_RED 17
#define LED_GREEN 16
#define LED_BLUE 2

// Sensor de Luz (LDR/ADC)
#define LDR_SENSOR 4

// Flash Memory SPI (Dedicado)
#define FLASH_CS 5
#define FLASH_SCK 18
#define FLASH_MISO 19
#define FLASH_MOSI 23

// Botões
#define BOOT_BUTTON 0
#define RESET_BUTTON -1 // Conectado ao pino EN

// Power
#define VCC 5
#define GND -1

// PWM Channels
#define LEDC_CHANNEL_0 0
#define LEDC_CHANNEL_1 1
#define LEDC_CHANNEL_2 2
#define LEDC_CHANNEL_3 3
#define LEDC_CHANNEL_4 4
#define LEDC_CHANNEL_5 5
#define LEDC_CHANNEL_6 6
#define LEDC_CHANNEL_7 7
#define LEDC_CHANNEL_8 8
#define LEDC_CHANNEL_9 9
#define LEDC_CHANNEL_10 10
#define LEDC_CHANNEL_11 11
#define LEDC_CHANNEL_12 12
#define LEDC_CHANNEL_13 13
#define LEDC_CHANNEL_14 14
#define LEDC_CHANNEL_15 15

// ADC Channels
#define ADC1_CHANNEL_0 36
#define ADC1_CHANNEL_1 39
#define ADC1_CHANNEL_2 34
#define ADC1_CHANNEL_3 35
#define ADC1_CHANNEL_4 32
#define ADC1_CHANNEL_5 33
#define ADC1_CHANNEL_6 25
#define ADC1_CHANNEL_7 26
#define ADC1_CHANNEL_8 27
#define ADC1_CHANNEL_9 14
#define ADC1_CHANNEL_10 12
#define ADC1_CHANNEL_11 13
#define ADC2_CHANNEL_0 4
#define ADC2_CHANNEL_1 0
#define ADC2_CHANNEL_2 2
#define ADC2_CHANNEL_3 15
#define ADC2_CHANNEL_4 13
#define ADC2_CHANNEL_5 12
#define ADC2_CHANNEL_6 14
#define ADC2_CHANNEL_7 27
#define ADC2_CHANNEL_8 25
#define ADC2_CHANNEL_9 26
#define ADC2_CHANNEL_10 1
#define ADC2_CHANNEL_11 3

// DAC Channels
#define DAC1 25
#define DAC2 26

// Touch Pins
#define TOUCH_H1 32
#define TOUCH_H2 33
#define TOUCH_V1 27
#define TOUCH_V2 4

// Especificações do Display
#define TFT_WIDTH 320
#define TFT_HEIGHT 480
#define TFT_DRIVER ST7796

// Especificações do Touch
#define TOUCH_DRIVER XPT2046
#define TOUCH_WIDTH 320
#define TOUCH_HEIGHT 480

// Especificações do SD Card
#define SDCARD_SPI_SPEED SPI_MODE0

// Mapeamento de Analógico para Digital
#define A0 36
#define A1 39
#define A2 34
#define A3 35
#define A4 32
#define A5 33
#define A6 25
#define A7 26
#define A8 27
#define A9 14
#define A10 12
#define A11 13
#define A12 4
#define A13 0
#define A14 2
#define A15 15

// Definições de Hardware Específicas
#define HAS_TOUCH 1
#define HAS_SD_CARD 1
#define HAS_AUDIO 1
#define HAS_RGB_LED 1
#define HAS_LDR_SENSOR 1
#define HAS_BACKLIGHT 1

// Configuração do Display em Modo SPI
#define TFT_SPI_MODE SPI_MODE0
#define TFT_SPI_FREQUENCY 40000000

// Configuração do Touch em Modo SPI
#define TOUCH_SPI_MODE SPI_MODE0
#define TOUCH_SPI_FREQUENCY 8000000

// Configuração do SD Card em Modo SPI
#define SDCARD_SPI_MODE SPI_MODE0
#define SDCARD_SPI_FREQUENCY 25000000

#endif // pins_arduino_h
