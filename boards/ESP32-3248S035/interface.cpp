#include "interface.h"
#include "Arduino.h"
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "TFT_eSPI.h"
#include "Wire.h"

// Configuração do Display ST7796
TFT_eSPI tft = TFT_eSPI();

// Configuração do Touch XPT2046 (implementação futura)
// XPT2046_Touch touch(TOUCH_CS, TOUCH_IRQ);

// Variáveis globais para a placa
bool backlightState = true;
int ledRedValue = 0;
int ledGreenValue = 0;
int ledBlueValue = 0;
float ldrValue = 0;

void setupESP32_3248S035() {
    // Inicialização da Serial
    Serial.begin(115200);
    while (!Serial) { delay(10); }

    // Configuração dos pinos GPIO
    pinMode(0, INPUT_PULLUP);  // BOOT_BUTTON
    pinMode(-1, INPUT_PULLUP); // RESET_BUTTON

    // Configuração do Backlight
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH); // Ligar backlight inicialmente

    // Configuração do LED RGB
    pinMode(17, OUTPUT);
    pinMode(16, OUTPUT);
    pinMode(2, OUTPUT);
    analogWrite(17, 0);
    analogWrite(16, 0);
    analogWrite(2, 0);

    // Configuração do Sensor de Luz (LDR)
    pinMode(4, INPUT);

    // Configuração do Áudio
    pinMode(26, OUTPUT);
    pinMode(25, OUTPUT);
    digitalWrite(26, HIGH); // Desliga o amplificador inicialmente

    // Inicialização do Display ST7796
    initDisplay();

    // Inicialização do Touch XPT2046
    initTouch();

    // Inicialização do SD Card
    initSDCard();

    // Mensagem de inicialização
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("ESP32-3248S035");
    tft.setTextSize(1);
    tft.println("Bruce Framework");
    tft.println("Display: ST7796 320x480");
    tft.println("Touch: XPT2046");
    tft.println("SD Card: SPI");
    tft.println("Audio: SC8002B");
    tft.println("LED RGB");
    tft.println("LDR Sensor");
    delay(2000);
}

void initDisplay() {
    // Configuração específica para ST7796 em modo SPI
    tft.init();

    // Rotação 1 = Paisagem (320x480)
    tft.setRotation(1);

    // Preencher tela com preto
    tft.fillScreen(TFT_BLACK);

    // Configurar texto
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);

    // Exibir informações do display
    tft.setCursor(10, 10);
    tft.println("Display ST7796 Inicializado!");
    tft.setCursor(10, 40);
    tft.println("Resolucao: 320x480");
    tft.setCursor(10, 70);
    tft.println("Driver: ST7796");
    tft.setCursor(10, 100);
    tft.println("Modo: SPI");
    tft.setCursor(10, 130);
    tft.println("Frequencia: 40MHz");

    delay(2000);

    // Limpar tela
    tft.fillScreen(TFT_BLACK);

    // Calibração do Touch (se necessário)
    calibrateTouch();
}

void initTouch() {
    // Inicialização do Touch XPT2046
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Touch XPT2046 Inicializado!");
    tft.setCursor(10, 40);
    tft.println("Driver: XPT2046");
    tft.setCursor(10, 70);
    tft.println("Interface: SPI");
    tft.setCursor(10, 100);
    tft.println("Resolucao: 320x480");
    tft.setCursor(10, 130);
    tft.println("Tipo: Resistivo");

    delay(2000);

    // Limpar tela
    tft.fillScreen(TFT_BLACK);

    // Configuração dos pinos SPI para touch
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);

    // Configuração do pino IRQ
    pinMode(33, INPUT_PULLUP);

    // Mensagem de sucesso
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Touch Pronto!");
    delay(1000);

    // Limpar tela
    tft.fillScreen(TFT_BLACK);
}

void initSDCard() {
    // Inicialização do SD Card
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Iniciando SD Card...");
    tft.setTextSize(1);

    // Configuração dos pinos SPI para SD Card
    pinMode(2, OUTPUT);  // CS
    pinMode(14, OUTPUT); // SCK
    pinMode(13, OUTPUT); // MISO
    pinMode(15, OUTPUT); // MOSI

    // Desativar todos os dispositivos SPI
    digitalWrite(2, HIGH);  // SD Card CS
    digitalWrite(13, HIGH); // Touch CS

    delay(100);

    // Tentativa de inicialização
    if (!SD.begin(2)) { // SDCARD_CS = 2
        Serial.println("Falha ao inicializar SD Card!");
        tft.fillScreen(TFT_RED);
        tft.setCursor(10, 10);
        tft.setTextColor(TFT_WHITE, TFT_RED);
        tft.setTextSize(2);
        tft.println("SD Card Falhou!");
        tft.setTextSize(1);
        tft.println("Verifique o cartao e a conexao");
        delay(3000);
        return;
    }

    Serial.println("SD Card inicializado com sucesso!");

    // Obter informações do cartão
    uint32_t cardSize = SD.cardSize() / (1024 * 1024);
    uint32_t cardSizeMB = cardSize / (1024 * 1024);
    uint32_t cardSizeGB = cardSize / (1024 * 1024 * 1024);

    // Exibir informações
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("SD Card OK!");
    tft.setTextSize(1);

    tft.setCursor(10, 40);
    tft.println("Interface: SPI");
    tft.setCursor(10, 60);
    tft.println("Pinos: MOSI=15, MISO=13");
    tft.setCursor(10, 80);
    tft.println("SCK=14, CS=2");

    if (cardSizeGB > 0) {
        tft.setCursor(10, 100);
        tft.println("Capacidade: " + String(cardSizeGB) + "GB");
    } else {
        tft.setCursor(10, 100);
        tft.println("Capacidade: " + String(cardSizeMB) + "MB");
    }

    // Listar arquivos
    tft.setCursor(10, 120);
    tft.println("Arquivos:");

    File root = SD.open("/");
    if (root) {
        int fileCount = 0;
        while (true) {
            File entry = root.openNextFile();
            if (!entry) break;

            if (fileCount < 5) { // Mostrar apenas os 5 primeiros
                tft.setCursor(10, 140 + fileCount * 15);
                tft.println(entry.name());
            }
            fileCount++;
            entry.close();
        }
        root.close();

        tft.setCursor(10, 220);
        tft.println("Total: " + String(fileCount) + " arquivos");
    }

    delay(3000);
}

void calibrateTouch() {
    // Calibração do touch screen
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println("Calibrando Touch...");

    // Realiza a calibração (implementação específica)
    // Isso pode ser ajustado conforme as necessidades
    delay(1000);

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println("Touch Calibrado!");
    delay(1000);
}

void setBacklight(bool state) {
    backlightState = state;
    digitalWrite(27, state ? HIGH : LOW);
}

void toggleBacklight() {
    backlightState = !backlightState;
    digitalWrite(27, backlightState ? HIGH : LOW);
}

void setRGBLed(int red, int green, int blue) {
    ledRedValue = constrain(red, 0, 255);
    ledGreenValue = constrain(green, 0, 255);
    ledBlueValue = constrain(blue, 0, 255);

    analogWrite(17, ledRedValue);
    analogWrite(16, ledGreenValue);
    analogWrite(2, ledBlueValue);
}

void setRGBLedColor(String color) {
    // Definir cores pré-definidas
    if (color == "RED") {
        setRGBLed(255, 0, 0);
    } else if (color == "GREEN") {
        setRGBLed(0, 255, 0);
    } else if (color == "BLUE") {
        setRGBLed(0, 0, 255);
    } else if (color == "WHITE") {
        setRGBLed(255, 255, 255);
    } else if (color == "YELLOW") {
        setRGBLed(255, 255, 0);
    } else if (color == "CYAN") {
        setRGBLed(0, 255, 255);
    } else if (color == "MAGENTA") {
        setRGBLed(255, 0, 255);
    } else if (color == "ORANGE") {
        setRGBLed(255, 165, 0);
    } else if (color == "PURPLE") {
        setRGBLed(128, 0, 128);
    } else if (color == "PINK") {
        setRGBLed(255, 192, 203);
    } else if (color == "OFF") {
        setRGBLed(0, 0, 0);
    } else {
        // Cor inválida, apagar LED
        setRGBLed(0, 0, 0);
    }
}

void rainbowLED() {
    // Efeito arco-iris no LED RGB
    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 256; i++) {
            switch (j) {
                case 0: setRGBLed(i, 255 - i, 0); break; // Vermelho para Verde
                case 1: setRGBLed(255 - i, 0, i); break; // Verde para Azul
                case 2: setRGBLed(0, i, 255 - i); break; // Azul para Vermelho
            }
            delay(10);
        }
    }
    setRGBLed(0, 0, 0); // Apagar no final
}

void breatheLED() {
    // Efeito respiração no LED RGB
    for (int i = 0; i < 256; i++) {
        setRGBLed(i, i, i); // Branco com intensidade variável
        delay(10);
    }
    for (int i = 255; i >= 0; i--) {
        setRGBLed(i, i, i); // Branco com intensidade variável
        delay(10);
    }
    setRGBLed(0, 0, 0); // Apagar no final
}

void setAudio(bool enabled) { digitalWrite(26, enabled ? LOW : HIGH); }

void playTone(int frequency, int duration) {
    // Tocar um tom de frequência específica
    if (frequency > 0 && duration > 0) {
        setAudio(true);
        tone(25, frequency, duration);
        delay(duration);
        noTone(25);
        setAudio(false);
    }
}

void playMelody() {
    // Tocar uma melodia simples
    int melody[] = {262, 294, 330, 349, 392, 440, 494, 523};
    int noteDuration = 200;

    setAudio(true);
    for (int i = 0; i < 8; i++) {
        tone(25, melody[i], noteDuration);
        delay(noteDuration);
        noTone(25);
    }
    setAudio(false);
}

void playErrorSound() {
    // Tocar som de erro
    playTone(200, 100);
    delay(50);
    playTone(150, 100);
}

void playSuccessSound() {
    // Tocar som de sucesso
    playTone(523, 100);
    delay(50);
    playTone(659, 100);
    delay(50);
    playTone(784, 200);
}

void readLDR() {
    // Leitura do sensor de luz (LDR)
    int rawValue = analogRead(4);
    ldrValue = (rawValue / 4095.0) * 100.0; // Converte para porcentagem

    // Calcular lux (aproximação)
    float voltage = (rawValue / 4095.0) * 3.3;
    float lux = 10000 * pow(voltage / 3.3, -1.5); // Fórmula aproximada para LDR

    // Armazenar valores globais
    ldrValue = constrain(ldrValue, 0.0, 100.0);
}

void calibrateLDR() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Calibrando LDR...");
    tft.setTextSize(1);

    // Leitura inicial
    readLDR();
    float minLight = ldrValue;
    float maxLight = ldrValue;

    // Coletar amostras por 5 segundos
    for (int i = 0; i < 50; i++) {
        readLDR();
        minLight = min(minLight, ldrValue);
        maxLight = max(maxLight, ldrValue);
        delay(100);

        // Mostrar progresso
        tft.setCursor(10, 40);
        tft.println("Amostra: " + String(i + 1) + "/50");
        tft.setCursor(10, 60);
        tft.println("Min: " + String(minLight, 1) + "%");
        tft.setCursor(10, 80);
        tft.println("Max: " + String(maxLight, 1) + "%");
    }

    // Armazenar calibração
    // (pode ser expandido para salvar em EEPROM ou arquivo)

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Calibracao Concluida!");
    tft.setTextSize(1);
    tft.setCursor(10, 40);
    tft.println("Faixa: " + String(minLight, 1) + " - " + String(maxLight, 1) + "%");
    delay(2000);

    tft.fillScreen(TFT_BLACK);
}

void autoAdjustBacklight() {
    // Ajustar backlight automaticamente baseado no LDR
    readLDR();

    // Definir níveis de backlight baseado na luz ambiente
    if (ldrValue < 20) {
        setBacklight(true); // Muito escuro - ligar backlight
    } else if (ldrValue < 40) {
        setBacklight(true); // Escuro - ligar backlight
    } else if (ldrValue < 60) {
        setBacklight(true); // Meia luz - ligar backlight
    } else if (ldrValue < 80) {
        setBacklight(false); // Claro - desligar backlight
    } else {
        setBacklight(false); // Muito claro - desligar backlight
    }
}

void displaySystemInfo() {
    // Exibe informações do sistema
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);

    tft.println("=== Sistema ESP32-3248S035 ===");
    tft.println("Display: ST7796 320x480");
    tft.println("Touch: XPT2046 Resistivo");
    tft.println("SD Card: SPI");
    tft.println("Audio: SC8002B");
    tft.println("LED RGB: " + String(ledRedValue) + "," + String(ledGreenValue) + "," + String(ledBlueValue));
    tft.println("Backlight: " + String(backlightState ? "ON" : "OFF"));
    tft.println("LDR: " + String(ldrValue, 1) + "%");
    tft.println("Bruce Framework v2.0");
}

// Funções de utilidade
void testDisplay() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Teste Display + Backlight");
    tft.setTextSize(1);

    // Teste de cores
    tft.println("Vermelho...");
    tft.fillScreen(TFT_RED);
    delay(500);

    tft.println("Verde...");
    tft.fillScreen(TFT_GREEN);
    delay(500);

    tft.println("Azul...");
    tft.fillScreen(TFT_BLUE);
    delay(500);

    tft.println("Branco...");
    tft.fillScreen(TFT_WHITE);
    delay(500);

    tft.fillScreen(TFT_BLACK);
    tft.println("Preto...");
    delay(500);

    // Teste de backlight
    tft.println("Testando Backlight...");
    tft.println("Ligando...");
    setBacklight(true);
    delay(1000);

    tft.println("Desligando...");
    setBacklight(false);
    delay(1000);

    tft.println("Ligando...");
    setBacklight(true);
    delay(1000);

    tft.fillScreen(TFT_BLACK);
    tft.println("Teste Concluido!");
    delay(1000);

    tft.fillScreen(TFT_BLACK);
}

void testTouch() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Teste Touch XPT2046");
    tft.setTextSize(1);
    tft.println("Toque na tela para testar...");

    int touchCount = 0;
    unsigned long lastTouchTime = 0;

    while (true) {
        // Verificar toque (implementação básica)
        if (digitalRead(33) == LOW) { // IRQ pino
            unsigned long currentTime = millis();
            if (currentTime - lastTouchTime > 500) { // Debounce
                touchCount++;
                lastTouchTime = currentTime;

                // Limpar tela e mostrar contador
                tft.fillScreen(TFT_BLACK);
                tft.setCursor(10, 10);
                tft.setTextColor(TFT_WHITE, TFT_BLACK);
                tft.setTextSize(2);
                tft.println("Toque Detectado!");
                tft.setTextSize(1);
                tft.println("Contador: " + String(touchCount));
                tft.println("Toque novamente...");

                delay(100);
            }
        }

        // Sair após 10 toques ou 30 segundos
        if (touchCount >= 10 || millis() > 30000) { break; }

        delay(10);
    }

    // Mensagem final
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Teste Touch Concluido!");
    tft.setTextSize(1);
    tft.println("Toques detectados: " + String(touchCount));
    delay(2000);
}

void testSDCard() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Teste SD Card");
    tft.setTextSize(1);
    tft.println("Verificando conexao...");

    // Teste de leitura
    if (!SD.begin(2)) {
        tft.fillScreen(TFT_RED);
        tft.setCursor(10, 10);
        tft.setTextColor(TFT_WHITE, TFT_RED);
        tft.setTextSize(2);
        tft.println("SD Card NAO RESPONDE!");
        delay(3000);
        return;
    }

    tft.println("Conexao OK!");
    delay(1000);

    // Teste de escrita
    tft.println("Testando escrita...");
    File testFile = SD.open("/test.txt", FILE_WRITE);
    if (testFile) {
        testFile.println("Teste ESP32-3248S035 - Bruce Framework");
        testFile.println("Data: " + String(__DATE__) + " " + String(__TIME__));
        testFile.close();
        tft.println("Escrita OK!");
    } else {
        tft.println("Falha na escrita!");
        delay(2000);
        return;
    }

    delay(1000);

    // Teste de leitura
    tft.println("Testando leitura...");
    testFile = SD.open("/test.txt");
    if (testFile) {
        tft.println("Arquivo lido:");
        while (testFile.available()) { tft.write(testFile.read()); }
        testFile.close();
    } else {
        tft.println("Falha na leitura!");
        delay(2000);
        return;
    }

    delay(1000);

    // Teste de listagem
    tft.println("\nListando arquivos:");
    File root = SD.open("/");
    if (root) {
        int fileCount = 0;
        while (true) {
            File entry = root.openNextFile();
            if (!entry) break;

            if (fileCount < 8) { // Mostrar até 8 arquivos
                String fileInfo = String(entry.name()) + " - " + String(entry.size()) + " bytes";
                tft.println(fileInfo);
            }
            fileCount++;
            entry.close();
        }
        root.close();
        tft.println("\nTotal: " + String(fileCount) + " arquivos");
    }

    delay(2000);

    // Limpar tela
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("SD Card OK!");
    delay(2000);
}

void testAudio() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Teste Audio");
    tft.setTextSize(1);
    tft.println("Amplificador SC8002B");
    delay(1000);

    // Teste básico
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println("1. Teste Basico...");
    setAudio(true);
    delay(1000);
    setAudio(false);
    delay(500);

    // Teste de tom
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println("2. Teste de Tom...");
    playTone(440, 500); // Lá (440Hz)
    delay(500);

    // Teste de melodia
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println("3. Teste de Melodia...");
    playMelody();
    delay(500);

    // Teste de som de erro
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println("4. Som de Erro...");
    playErrorSound();
    delay(500);

    // Teste de som de sucesso
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println("5. Som de Sucesso...");
    playSuccessSound();
    delay(500);

    // Teste de diferentes frequências
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println("6. Teste de Frequencias...");
    int frequencies[] = {262, 330, 392, 523, 659, 784}; // Notas musicais
    for (int i = 0; i < 6; i++) {
        tft.setCursor(10, 40 + i * 15);
        tft.println("Nota: " + String(frequencies[i]) + "Hz");
        playTone(frequencies[i], 200);
        delay(300);
    }

    // Teste de durações
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println("7. Teste de Duracoes...");
    int durations[] = {100, 200, 500, 1000};
    for (int i = 0; i < 4; i++) {
        tft.setCursor(10, 40 + i * 15);
        tft.println("Duracao: " + String(durations[i]) + "ms");
        playTone(440, durations[i]);
        delay(200);
    }

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Teste Audio Concluido!");
    delay(2000);

    tft.fillScreen(TFT_BLACK);
}

String getRGBValues(String color) {
    // Retorna os valores RGB como string para exibição
    if (color == "RED") return "255, 0, 0";
    if (color == "GREEN") return "0, 255, 0";
    if (color == "BLUE") return "0, 0, 255";
    if (color == "WHITE") return "255, 255, 255";
    if (color == "YELLOW") return "255, 255, 0";
    if (color == "CYAN") return "0, 255, 255";
    if (color == "MAGENTA") return "255, 0, 255";
    if (color == "ORANGE") return "255, 165, 0";
    if (color == "PURPLE") return "128, 0, 128";
    if (color == "PINK") return "255, 192, 203";
    return "0, 0, 0";
}

void testRGBLed() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Teste LED RGB");
    tft.setTextSize(1);
    tft.println("Testando cores basicas...");
    delay(1000);

    // Teste de cores básicas
    String colors[] = {
        "RED", "GREEN", "BLUE", "WHITE", "YELLOW", "CYAN", "MAGENTA", "ORANGE", "PURPLE", "PINK"
    };
    int colorCount = sizeof(colors) / sizeof(colors[0]);

    for (int i = 0; i < colorCount; i++) {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(10, 10);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setTextSize(2);
        tft.println("Cor: " + colors[i]);
        tft.setTextSize(1);
        tft.println("RGB: " + String(getRGBValues(colors[i])));

        setRGBLedColor(colors[i]);
        delay(800);
    }

    // Teste de apagamento
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Apagando LED...");
    setRGBLedColor("OFF");
    delay(1000);

    // Teste de efeitos
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Efeito: Respiracao");
    breatheLED();

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Efeito: Arco-Iris");
    rainbowLED();

    // Finalizar
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Teste Concluido!");
    setRGBLedColor("OFF");
    delay(2000);

    tft.fillScreen(TFT_BLACK);
}

void testLDR() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Teste LDR");
    tft.setTextSize(1);
    tft.println("Sensor de Luz Ambiente");
    delay(2000);

    // Teste de leitura contínua
    for (int i = 0; i < 20; i++) {
        readLDR();

        // Limpar área de texto
        tft.fillRect(10, 40, 300, 120, TFT_BLACK);

        tft.setCursor(10, 40);
        tft.println("Leitura " + String(i + 1) + "/20");
        tft.setCursor(10, 60);
        tft.println("Luz: " + String(ldrValue, 1) + "%");

        // Calcular lux
        float voltage = (analogRead(4) / 4095.0) * 3.3;
        float lux = 10000 * pow(voltage / 3.3, -1.5);
        tft.setCursor(10, 80);
        tft.println("Lux: " + String(lux, 0));

        // Classificar ambiente
        String ambiente;
        if (ldrValue < 20) {
            ambiente = "Muito Escuro";
        } else if (ldrValue < 40) {
            ambiente = "Escuro";
        } else if (ldrValue < 60) {
            ambiente = "Meia Luz";
        } else if (ldrValue < 80) {
            ambiente = "Claro";
        } else {
            ambiente = "Muito Claro";
        }
        tft.setCursor(10, 100);
        tft.println("Ambiente: " + ambiente);

        // Barra visual
        int barWidth = (ldrValue / 100.0) * 200;
        tft.fillRect(10, 120, barWidth, 10, TFT_GREEN);
        tft.drawRect(10, 120, 200, 10, TFT_WHITE);

        delay(500);
    }

    // Teste de calibração automática
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Teste Auto-Backlight");
    tft.setTextSize(1);
    tft.println("Ajustando backlight...");

    for (int i = 0; i < 5; i++) {
        autoAdjustBacklight();
        tft.setCursor(10, 40);
        tft.println("Luz: " + String(ldrValue, 1) + "%");
        tft.setCursor(10, 60);
        tft.println("Backlight: " + String(backlightState ? "ON" : "OFF"));
        delay(1000);
    }

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Teste LDR Concluido!");
    delay(2000);

    tft.fillScreen(TFT_BLACK);
}

// Função principal de teste
void runHardwareTests() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Testes Hardware");
    tft.setTextSize(1);

    tft.println("1. Display...");
    testDisplay();

    tft.println("2. Touch...");
    testTouch();

    tft.println("3. SD Card...");
    testSDCard();

    tft.println("4. Audio...");
    testAudio();

    tft.println("5. LED RGB...");
    testRGBLed();

    tft.println("6. LDR...");
    testLDR();

    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.println("Testes Concluidos!");
    delay(2000);
}
