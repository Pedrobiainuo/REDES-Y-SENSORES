#include <Wire.h>
#include "Adafruit_SGP30.h"

#define LED_VERDE 48
#define SDA_PIN 21
#define SCL_PIN 20

Adafruit_SGP30 sgp;

// Intervalos
const float intervalo_lectura = 0.1;   // segundos
const float intervalo_media = 5.0;     // segundos
const int num_muestras = intervalo_media / intervalo_lectura; // 50 lecturas

// Variables acumuladoras
float suma_co2 = 0;
float suma_tvoc = 0;
int contador = 0;

// Temporizadores
unsigned long t_ultimo = 0;
unsigned long t_led = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);

  pinMode(LED_VERDE, OUTPUT);
  digitalWrite(LED_VERDE, LOW);

  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.println("Inicializando SGP30...");

  if (!sgp.begin()) {
    Serial.println("❌ No se encontró el SGP30. Verifica conexión I2C.");
    while (1);
  }

  sgp.IAQinit();
  Serial.println("✅ SGP30 inicializado correctamente.");
  Serial.println("Midiendo cada 0.1 s y mostrando media cada 5 s...\n");
}

void loop() {
  unsigned long ahora = millis();

  // Parpadeo LED
  if (ahora - t_led >= 100) {
    digitalWrite(LED_VERDE, !digitalRead(LED_VERDE));
    t_led = ahora;
  }

  // Lectura del sensor
  if (ahora - t_ultimo >= 100) {
    t_ultimo = ahora;

    if (sgp.IAQmeasure()) {
      suma_co2 += sgp.eCO2;
      suma_tvoc += sgp.TVOC;
      contador++;
    }

    // Cada 5 s → mostrar medias
    if (contador >= num_muestras) {
      float media_co2 = suma_co2 / contador;
      float media_tvoc = suma_tvoc / contador;

      Serial.print("Media 5s → CO2: ");
      Serial.print(media_co2, 1);
      Serial.print(" ppm | TVOC: ");
      Serial.print(media_tvoc, 1);
      Serial.println(" ppb");

      // También puedes enviar en formato CSV para Python
      Serial.print("DATA,");
      Serial.print(media_co2, 1);
      Serial.print(",");
      Serial.println(media_tvoc, 1);

      suma_co2 = suma_tvoc = 0;
      contador = 0;
    }
  }
}
