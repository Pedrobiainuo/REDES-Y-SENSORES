#include <Wire.h>
#include <WiFi.h>
#include "Adafruit_SGP30.h"
#include <ArduinoJson.h>
#include "time.h"   // Para obtener la hora UTC desde internet

// ============================================================
// CONFIGURACIÓN GENERAL
// ============================================================

// 🧭 Pines I2C (ajústalos según tu conexión real)
#define SDA_PIN 21
#define SCL_PIN 20

Adafruit_SGP30 sgp;

// ⚙️ Configuración WiFi
const char* ssid = "TP-LINK_FBF0_5G";
const char* password = "58408148";

// Identificador del módulo
const char* deviceID = "B8";

// Servidor NTP para hora UTC
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;

// Configuración del sensor MQ-7B
#define MQ7B_PIN 1
#define MQ_VCC 5.0
const float RL = 10.0;   // Resistencia de carga en kΩ
const float R0 = 10.0;
const float A = -0.77;
const float B = 1.47;

// ============================================================
// FUNCIONES AUXILIARES
// ============================================================

// 🕓 Devuelve tiempo UTC en formato "YYYY-DoY-HH:MM:SS"
String getUTCTimeString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("⚠️ No se pudo obtener la hora UTC");
    return "0000-000-00:00:00";
  }

  char buffer[25];
  int year = timeinfo.tm_year + 1900;
  int yday = timeinfo.tm_yday + 1;  // Día del año (1–366)
  int hour = timeinfo.tm_hour;
  int minute = timeinfo.tm_min;
  int second = timeinfo.tm_sec;

  sprintf(buffer, "%04d-%03d-%02d:%02d:%02d", year, yday, hour, minute, second);
  return String(buffer);
}

// 🧮 CONVERSIÓN MQ-7B: Voltaje a PPM de CO
float voltageToPPM(float voltage) {
  if (voltage <= 0.01) return 0.0;
  if (voltage >= MQ_VCC) voltage = MQ_VCC - 0.01;

  float RS = RL * (MQ_VCC - voltage) / voltage;
  float ratio = RS / R0;
  if (ratio <= 0) return 0.0;
  return pow(10, (A * log10(ratio) + B));
}

// ============================================================
// SETUP
// ============================================================

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("=======================================");
  Serial.println("🚀 INICIANDO MÓDULO ESP32 + SGP30 + MQ-7B");
  Serial.println("=======================================");

  // 🔌 Conexión WiFi
  Serial.print("Conectando a WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi conectado");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());

  // 🕐 Sincronizar hora UTC
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("🕓 Hora UTC sincronizada");

  // 🔌 Inicializar bus I2C con tus pines definidos
  Wire.begin(SDA_PIN, SCL_PIN);
  Serial.print("🧩 I2C inicializado en SDA=");
  Serial.print(SDA_PIN);
  Serial.print(", SCL=");
  Serial.println(SCL_PIN);

  // 🔬 Inicializar sensor SGP30
  if (!sgp.begin(&Wire)) {
    Serial.println("❌ Error al inicializar el SGP30. Verifica conexiones.");
    while (1);
  }

  sgp.IAQinit();  // Calibración inicial
  Serial.println("✅ Sensor SGP30 iniciado correctamente");
  
  // Configuración ADC para el sensor MQ-7B
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db); // Rango completo 0-3.3V (aprox)
}

// ============================================================
// LOOP
// ============================================================

void loop() {
  // Medición IAQ del SGP30
  if (!sgp.IAQmeasure()) {
    Serial.println("⚠️ Error al leer el sensor SGP30");
    delay(1000);
    return;
  }

  // Leer voltaje del sensor MQ-7B
  float voltage_pin = analogReadMilliVolts(MQ7B_PIN) / 1000.0;
  float ppm_CO = voltageToPPM(voltage_pin); // CO en PPM

  // 🧮 Crear documento JSON con formato legible
  StaticJsonDocument<512> jsonDoc;
  jsonDoc["ID"] = deviceID;
  jsonDoc["Tiempo_UTC"] = getUTCTimeString();
  jsonDoc["CO2_ppm"] = sgp.eCO2;
  jsonDoc["TVOC_ppb"] = sgp.TVOC;
  jsonDoc["CO_ppm"] = (int)ppm_CO;

  // Convertir a cadena JSON
  String jsonString;
  serializeJsonPretty(jsonDoc, jsonString);

  // 📟 Mostrar por Serial
  Serial.println(jsonString);
  Serial.println("---------------------------------------");

  delay(2000);  // Cada 2 segundos
}

