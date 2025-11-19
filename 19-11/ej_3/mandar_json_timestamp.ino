#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "Adafruit_SGP30.h"
#include <ArduinoJson.h>
#include "time.h"

// ============================================================
// CONFIGURACIÓN GENERAL
// ============================================================
#define SDA_PIN 21
#define SCL_PIN 20

Adafruit_SGP30 sgp;

// ⚙️ Config WiFi
const char* ssid = "TP-LINK_FBF0_5G";
const char* password = "58408148";

// MQTT
const char* mqtt_broker = "10.42.0.1";
const uint16_t mqtt_port = 1883;
// Topic solicitado (tiene espacios; PubSubClient lo acepta)
const char* mqtt_topic = "Enviromental Sensors Network";
WiFiClient espClient;
PubSubClient mqtt(espClient);

// Identificador del módulo
const char* deviceID = "B8";

// NTP
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 0;

// MQ-7B
#define MQ7B_PIN 1
#define MQ_VCC 5.0
const float RL = 10.0;   // kΩ
const float R0 = 10.0;
const float A = -0.77;
const float B = 1.47;

// Intervalo de envío (20 segundos)
const unsigned long SEND_INTERVAL_MS = 20000;
unsigned long lastSend = 0;

String getUTCTimeString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "0000-000-00:00:00";
  }
  char buffer[25];
  int year = timeinfo.tm_year + 1900;
  int yday = timeinfo.tm_yday + 1;
  sprintf(buffer, "%04d-%03d-%02d:%02d:%02d", year, yday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  return String(buffer);
}

float voltageToPPM(float voltage) {
  if (voltage <= 0.01) return 0.0;
  if (voltage >= MQ_VCC) voltage = MQ_VCC - 0.01;
  float RS = RL * (MQ_VCC - voltage) / voltage;
  float ratio = RS / R0;
  if (ratio <= 0) return 0.0;
  return pow(10, (A * log10(ratio) + B));
}

void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    // timeout opcional tras 20s para reintentar
    if (millis() - start > 20000) break;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print("WiFi conectado. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println("No se pudo conectar a WiFi (reintentará más tarde).");
  }
}

void mqttReconnect() {
  if (mqtt.connected()) return;
  Serial.print("Conectando MQTT...");
  String clientId = String(deviceID) + "-" + String(random(0xffff), HEX);
  if (mqtt.connect(clientId.c_str())) {
    Serial.println("conectado");
  } else {
    Serial.print("falló, rc=");
    Serial.print(mqtt.state());
    Serial.println(" - reintentando en 2s");
    delay(2000);
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Iniciando ESP32 + SGP30 + MQ-7B + MQTT (envío cada 20s)");

  connectWiFi();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Wire.begin(SDA_PIN, SCL_PIN);

  if (!sgp.begin(&Wire)) {
    Serial.println("Error al inicializar SGP30");
    while (1) delay(1000);
  }
  sgp.IAQinit();

  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  mqtt.setServer(mqtt_broker, mqtt_port);

  lastSend = millis();
}

void loop() {
  // Mantener conexión WiFi/MQTT
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqtt.connected()) mqttReconnect();
  mqtt.loop();

  // Leer sensor SGP30 cada ciclo y publicar cada 20s
  if (!sgp.IAQmeasure()) {
    Serial.println("Error al leer SGP30");
    delay(1000);
    return;
  }

  // Calcular CO desde MQ-7B
  float voltage_pin = analogReadMilliVolts(MQ7B_PIN) / 1000.0;
  float ppm_CO = voltageToPPM(voltage_pin);

  // Preparar JSON
  StaticJsonDocument<512> jsonDoc;
  jsonDoc["ID"] = deviceID;
  jsonDoc["Tiempo_UTC"] = getUTCTimeString();
  jsonDoc["CO2_ppm"] = sgp.eCO2;
  jsonDoc["TVOC_ppb"] = sgp.TVOC;
  jsonDoc["CO_ppm"] = (int)ppm_CO;

  // Mostrar por Serial (legible)
  String pretty;
  serializeJsonPretty(jsonDoc, pretty);
  Serial.println(pretty);
  Serial.println("---------------------------------------");

  // Publicar solo cada SEND_INTERVAL_MS
  if (millis() - lastSend >= SEND_INTERVAL_MS) {
    lastSend = millis();
    // serialize a String y publicar (PubSubClient usa QoS 0)
    String payload;
    serializeJson(jsonDoc, payload);
    if (mqtt.connected()) {
      bool ok = mqtt.publish(mqtt_topic, payload.c_str());
      Serial.print("Publicado MQTT en '");
      Serial.print(mqtt_topic);
      Serial.print("' -> ");
      Serial.println(ok ? "OK" : "FALLÓ");
    } else {
      Serial.println("MQTT no conectado. No se publicó.");
    }
  }

  // Pequeña espera para evitar lecturas excesivas
  delay(500);
}