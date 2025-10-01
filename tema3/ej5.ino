#include <Adafruit_NeoPixel.h>

#define LED_PIN 13    // Pin GPIO conectado al LED RGBW
#define NUM_LEDS 1    // Número de LEDs
#define TOUCH_PIN T0  // Pin táctil (GPIO 4 = Touch0)

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  Serial.begin(115200);  // Inicia comunicación serial para depuración
  strip.begin();         // Inicializa la tira de NeoPixel
  strip.show();          // Apaga todos los píxeles inicialmente
}

void loop() {
  int touchValue = touchRead(TOUCH_PIN);  // Lee el valor del pin táctil
  Serial.println(touchValue);  // Imprime el valor para depuración

  // Umbral de detección de toque (ajusta según pruebas, normalmente < 40 indica toque)
  if (touchValue < 40) {
    strip.setPixelColor(0, 0, 0, 0, 255);  // Enciende blanco
  } else {
    strip.setPixelColor(0, 0, 0, 0, 0);    // Apaga el LED
  }
  strip.show();  // Actualiza el color del LED
  delay(100);    // Pequeña pausa para estabilidad
}