#include <Wire.h>
#include "Adafruit_SGP30.h"
#include <Adafruit_NeoPixel.h>

// Pines
#define SDA_PIN 21
#define SCL_PIN 20
#define PIN_RGB 48   // LED RGB del ESP32-S3 DevKitC-1 (ajusta si usas otro)

// Objetos
Adafruit_SGP30 sgp;
Adafruit_NeoPixel led(1, PIN_RGB, NEO_GRB + NEO_KHZ800);

// Umbrales
const int CO2_MIN = 400;
const int CO2_MAX = 1000;
const int TVOC_MAX = 500;

// Tiempos
unsigned long t_ultimo = 0;
const unsigned long intervalo_lectura = 1000; // ms

// Funciones auxiliares
void setColor(uint8_t r, uint8_t g, uint8_t b) {
  led.setPixelColor(0, led.Color(r, g, b));
  led.show();
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  // Inicializar LED RGB
  led.begin();
  setColor(0, 0, 0); // apagado

  // Inicializar I²C y SGP30
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.println("Inicializando SGP30...");

  if (!sgp.begin()) {
    Serial.println("❌ No se encontró el SGP30. Verifica la conexión I2C.");
    while (1);
  }

  sgp.IAQinit();
  Serial.println("✅ SGP30 inicializado correctamente.");
  Serial.println("Midiendo cada 1 s y cambiando color LED según estado...\n");
}

void loop() {
  unsigned long ahora = millis();

  if (ahora - t_ultimo >= intervalo_lectura) {
    t_ultimo = ahora;

    if (sgp.IAQmeasure()) {
      int co2 = sgp.eCO2;
      int tvoc = sgp.TVOC;

      Serial.print("CO2: ");
      Serial.print(co2);
      Serial.print(" ppm | TVOC: ");
      Serial.print(tvoc);
      Serial.println(" ppb");

      // Evaluar si hay alarma
      bool alarma = (co2 > CO2_MAX) || (tvoc > TVOC_MAX);

      if (alarma) {
        Serial.println("⚠️  ¡ALERTA! Niveles fuera de rango.");
        setColor(255, 0, 0); // rojo
      } else {
        setColor(0, 255, 0); // verde
      }
    }
  }
}
