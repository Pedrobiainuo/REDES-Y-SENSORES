#include <Wire.h>
#include <Adafruit_SGP30.h>

// Configura el pin SDA en el pin 21 y SCL en el pin 20
#define SDA_PIN 21
#define SCL_PIN 20

Adafruit_SGP30 sgp;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);  // Configura los pines SDA y SCL
  Serial.println("Conectando al sensor SGP30...");
  
  // Intentar inicializar el sensor varias veces
  while (!sgp.begin()) {
    Serial.println("Error al inicializar el SGP30. Intentando nuevamente...");
    delay(1000);  // Esperar 1 segundo antes de intentar nuevamente
  }

  Serial.println("SGP30 inicializado correctamente.");
}

void loop() {
  uint16_t eCO2, TVOC;
  if (sgp.IAQmeasure()) {
    eCO2 = sgp.eCO2;
    TVOC = sgp.TVOC;
    Serial.print("eCO2: ");
    Serial.print(eCO2);
    Serial.print(" ppm, TVOC: ");
    Serial.print(TVOC);
    Serial.println(" ppb");
  } else {
    Serial.println("Error al leer los datos del sensor.");
  }

  delay(5000);  // Medir cada 5 segundos
}
