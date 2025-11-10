# ESP32-3248S035 - Bruce Framework Support

## Visão Geral

A placa ESP32-3248S035 é uma placa de desenvolvimento baseada no ESP32-WROOM-32 com display ST7796 de 320x480 pixels, touch resistivo XPT2046, slot SD Card, amplificador de áudio SC8002B, LED RGB e sensor de luz LDR. Esta documentação descreve o suporte completo para esta placa no Bruce Framework.

## Especificações Técnicas

### Hardware
- **Microcontrolador**: ESP32-WROOM-32 (Dual-core, Wi-Fi, Bluetooth)
- **Display**: ST7796 320x480 pixels, interface SPI
- **Touch**: XPT2046 resistivo, interface SPI
- **SD Card**: Interface SPI (compartilhada com display)
- **Áudio**: Amplificador SC8002B
- **LED RGB**: LED de 3 cores (Vermelho, Verde, Azul)
- **Sensor**: LDR (Sensor de luz ambiente)
- **Memória Flash**: 25Q32 externo (32MB)

### Pinagem

#### Display ST7796 (SPI)
- TFT_MOSI: 13
- TFT_SCLK: 14
- TFT_CS: 15
- TFT_DC: 21
- TFT_RST: 4
- TFT_BACKLIGHT: 27

#### Touch XPT2046 (SPI)
- TOUCH_CS: 13
- TOUCH_CLK: 14
- TOUCH_DIN: 12
- TOUCH_DOUT: 32
- TOUCH_IRQ: 33

#### SD Card (SPI)
- SDCARD_CS: 2
- SDCARD_SCK: 14
- SDCARD_MISO: 13
- SDCARD_MOSI: 15

#### I2C
- I2C_SDA: 22
- I2C_SCL: 23

#### Audio
- AUDIO_SHUTDOWN: 26
- AUDIO_IN: 25

#### LED RGB
- LED_RED: 17
- LED_GREEN: 16
- LED_BLUE: 2

#### Sensor
- LDR_SENSOR: 4

#### Botões
- BOOT_BUTTON: 0
- RESET_BUTTON: -1 (conectado ao pino EN)

## Configuração do Bruce Framework

### Arquivos Criados

1. **ESP32-3248S035.ini** - Configuração do PlatformIO
2. **pins_arduino.h** - Definições de pinos
3. **interface.h** - Declarações de funções
4. **interface.cpp** - Implementação das funções
5. **board.json** - Configuração do board
6. **README.md** - Documentação

### Bibliotecas Necessárias

As seguintes bibliotecas são necessárias para o funcionamento completo:

```ini
lib_deps =
    bodmer/TFT_eSPI @ ^2.5.43
    adafruit/Adafruit XPT2046 Touch Library @ ^1.0.0
    paulstoffregen/SD @ ^1.2.4
    adafruit/Adafruit GFX Library @ ^1.11.9
```

### Configuração do Display

O display ST7796 é configurado em modo SPI com as seguintes características:

```cpp
tft.init();
tft.setRotation(1); // Paisagem
tft.fillScreen(TFT_BLACK);
```

### Configuração do Touch

O touch XPT2046 é configurado em modo SPI:

```cpp
touch.begin();
touch.setRotation(1); // Mesma rotação do display
```

### Configuração do SD Card

O SD Card é inicializado com a seguinte configuração:

```cpp
if (!SD.begin(SDCARD_CS)) {
    // Tratar erro
}
```

## Conflitos de Hardware

Devido ao design da placa, existem conflitos de pinos que precisam ser considerados:

### Conflitos Principais
1. **Display e Touch**: Compartilham os pinos SPI (MOSI, SCLK)
2. **Display e SD Card**: Compartilham os pinos SPI (MOSI, SCLK, MISO)
3. **Touch e SD Card**: Compartilham os pinos SPI (MOSI, SCLK)

### Solução
Os dispositivos são selecionados individualmente através dos pinos Chip Select (CS). A biblioteca TFT_eSPI gerencia automaticamente os pinos CS do display.

## Funções Disponíveis

### Funções de Setup
- `setupESP32_3248S035()` - Setup inicial da placa
- `initDisplay()` - Inicialização do display
- `initTouch()` - Inicialização do touch
- `initSDCard()` - Inicialização do SD Card
- `calibrateTouch()` - Calibração do touch

### Funções de Controle
- `setBacklight(bool state)` - Controle do backlight
- `toggleBacklight()` - Alterna o estado do backlight
- `setRGBLed(int red, int green, int blue)` - Controle do LED RGB
- `setRGBLedColor(String color)` - Define cor pré-definida do LED RGB
- `setAudio(bool enabled)` - Controle do áudio
- `readLDR()` - Leitura do sensor de luz
- `calibrateLDR()` - Calibração do sensor LDR
- `autoAdjustBacklight()` - Ajuste automático do backlight baseado no LDR

### Funções de Efeito LED
- `rainbowLED()` - Efeito arco-iris no LED RGB
- `breatheLED()` - Efeito respiração no LED RGB

### Funções de Áudio
- `playTone(int frequency, int duration)` - Toca um tom específico
- `playMelody()` - Toca uma melodia pré-definida
- `playErrorSound()` - Toca som de erro
- `playSuccessSound()` - Toca som de sucesso

### Funções de Sistema
- `displaySystemInfo()` - Exibe informações do sistema
- `getRGBValues(String color)` - Retorna valores RGB de uma cor

### Funções de Teste
- `testDisplay()` - Teste do display
- `testTouch()` - Teste do touch
- `testSDCard()` - Teste do SD Card
- `testAudio()` - Teste do áudio
- `testRGBLed()` - Teste do LED RGB
- `testLDR()` - Teste do sensor LDR
- `runHardwareTests()` - Executa todos os testes

## Exemplos de Uso

### Exemplo 1: Setup Básico

```cpp
#include "interface.h"

void setup() {
    setupESP32_3248S035();

    // Configurar LED RGB para branco
    setRGBLed(255, 255, 255);

    // Ligar backlight
    setBacklight(true);

    // Ligar áudio
    setAudio(true);

    // Exibir mensagem de boas-vindas
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("ESP32-3248S035");
    tft.println("Bruce Framework");
}

void loop() {
    // Manter o sistema atualizado
    displaySystemInfo();
    delay(1000);
}
```

### Exemplo 2: Sistema de Monitoramento Ambiental

```cpp
#include "interface.h"

void setup() {
    setupESP32_3248S035();

    // Configurar LED RGB para modo monitoramento
    setRGBLedColor("CYAN");

    // Ligar backlight
    setBacklight(true);

    // Inicializar SD Card para logging
    initSDCard();
}

void loop() {
    // Ler sensor de luz
    readLDR();

    // Ajustar backlight automaticamente
    autoAdjustBacklight();

    // Exibir informações ambientais
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);

    tft.println("=== Monitoramento Ambiental ===");
    tft.println("Luz Ambiente: " + String(ldrValue, 1) + "%");

    // Calcular lux
    float voltage = (analogRead(4) / 4095.0) * 3.3;
    float lux = 10000 * pow(voltage / 3.3, -1.5);
    tft.println("Lux: " + String(lux, 0));

    // Classificar ambiente
    String ambiente;
    if (ldrValue < 20) ambiente = "Muito Escuro";
    else if (ldrValue < 40) ambiente = "Escuro";
    else if (ldrValue < 60) ambiente = "Meia Luz";
    else if (ldrValue < 80) ambiente = "Claro";
    else ambiente = "Muito Claro";

    tft.println("Ambiente: " + ambiente);
    tft.println("Backlight: " + String(backlightState ? "ON" : "OFF"));

    delay(2000);
}
```

### Exemplo 3: Teste Automatizado de Hardware

```cpp
#include "interface.h"

void setup() {
    setupESP32_3248S035();

    // Executar testes automatizados
    runHardwareTests();

    // Configurar LED RGB para verde (sucesso)
    setRGBLedColor("GREEN");

    // Tocar som de sucesso
    playSuccessSound();
}

void loop() {
    // Manter LED RGB piscando
    breatheLED();

    delay(100);
}
```

### Exemplo 4: Sistema de Alertas Sonoros e Visuais

```cpp
#include "interface.h"

void setup() {
    setupESP32_3248S035();

    // Configurar LED RGB
    setRGBLedColor("WHITE");

    // Ligar backlight
    setBacklight(true);
}

void loop() {
    // Monitorar sensor de luz
    readLDR();

    // Sistema de alertas
    if (ldrValue < 10) {
        // Muito escuro - alerta vermelho
        setRGBLedColor("RED");
        playErrorSound();
        setBacklight(true);
    } else if (ldrValue > 90) {
        // Muito claro - alerta azul
        setRGBLedColor("BLUE");
        playTone(800, 200);
        setBacklight(false);
    } else {
        // Condição normal - verde
        setRGBLedColor("GREEN");
        setAudio(false);
    }

    delay(1000);
}
```

## Build e Upload

### PlatformIO Configuration

Adicione a seguinte configuração ao seu `platformio.ini`:

```ini
[env:ESP32-3248S035]
board = ESP32-3248S035
build_src_filter = ${env.build_src_filter} +<../boards/ESP32-3248S035>
-Iboards/ESP32-3248S035
-DDEVICE_NAME='"ESP32-3248S035"'
extends=env:ESP32-3248S028
${env:ESP32-3248S028.build_flags}
```

### Build Commands

```bash
# Build para ESP32-3248S035
pio run -e ESP32-3248S035

# Upload para a placa
pio run -e ESP32-3248S035 -t upload

# Monitor serial
pio device monitor
```

## Troubleshooting

### Problemas Comuns

1. **Display não funciona**: Verifique a conexão SPI e os pinos CS
2. **Touch não responde**: Verifique a calibração e conexão SPI
3. **SD Card não é reconhecido**: Verifique o pino CS e formatação
4. **LED RGB não acende**: Verifique os pinos PWM e conexão

### Dicas de Debug

1. Use a função `runHardwareTests()` para testar todos os componentes
2. Verifique as conexões físicas com o multímetro
3. Use o monitor serial para depuração
4. Teste cada componente individualmente

## Referências

- [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)
- [ST7796 Datasheet](https://www.buydisplay.com/download/datasheet/ST7796.pdf)
- [XPT2046 Datasheet](https://www.buydisplay.com/download/datasheet/XPT2046.pdf)
- [Bruce Framework](https://github.com/pr3y/Bruce)

## Contribuição

Para contribuir com melhorias ou correções, por favor:

1. Faça um fork do projeto
2. Crie uma branch para sua modificação
3. Faça o commit das suas mudanças
4. Abra um Pull Request

## Licença

Este projeto está sob a licença MIT. Veja o arquivo LICENSE para mais detalhes.
