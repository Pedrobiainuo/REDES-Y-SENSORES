#include <Wire.h>
#include <Adafruit_SGP30.h>

#define SDA_PIN 21
#define SCL_PIN 20

Adafruit_SGP30 sgp;

// Rango de CO₂ considerado normal
const int CO2_MIN_NORMAL = 400;   // ppm
const int CO2_MAX_NORMAL = 1000;  // ppm

void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("\nInicializando SGP30...");

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!sgp.begin()) {
    Serial.println("❌ No se detecta el sensor SGP30. Verifica conexión I2C (dirección 0x58).");
    while (1);
  }

  sgp.IAQinit();
  Serial.println("✅ SGP30 inicializado correctamente.\n");
}

void loop() {
  if (sgp.IAQmeasure()) {
    uint16_t eCO2 = sgp.eCO2;
    uint16_t TVOC = sgp.TVOC;

    Serial.print("CO₂: ");
    Serial.print(eCO2);
    Serial.print(" ppm | TVOC: ");
    Serial.print(TVOC);
    Serial.println(" ppb");

#ifdef RGB_BUILTIN
    // Cambia color del LED según el nivel de CO₂
    if (eCO2 < CO2_MIN_NORMAL) {
      // CO₂ muy bajo → azul (sensor recién encendido o calibrando)
      rgbLedWrite(RGB_BUILTIN, 0, 0, 255);
    } else if (eCO2 <= CO2_MAX_NORMAL) {
      // Aire normal → verde
      rgbLedWrite(RGB_BUILTIN, 0, 255, 0);
    } else {
      // CO₂ alto → rojo
      rgbLedWrite(RGB_BUILTIN, 255, 0, 0);
    }
#endif

  } else {
    Serial.println("⚠️ Error al leer el SGP30.");
#ifdef RGB_BUILTIN
    rgbLedWrite(RGB_BUILTIN, 255, 255, 0);  // Amarillo para indicar error
#endif
  }

  delay(1000);  // Mide cada segundo
}
