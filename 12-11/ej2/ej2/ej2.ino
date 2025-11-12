#include <WiFi.h>          // Para el módulo WiFi del ESP32S3
#include <PubSubClient.h>  // Para la comunicación MQTT
#include <ArduinoJson.h>   // Para el manejo de datos JSON

// --- 1. Credenciales y configuración ---
const char* ssid = "LunarComms4";
const char* password = "11223344";
const char* mqtt_server = "10.42.0.1";
const int mqtt_port = 1883;

// Tópicos y texto para el Apartado 2
const char* text_topic = "text_test";
const char* text_message = "¡Mensaje de prueba simple! (QoS 0)";

// Tópico para el Apartado 4 (JSON)
const char* json_topic = "sensor/data";

WiFiClient espClient;
PubSubClient client(espClient);

// --- Funciones de Conexión ---

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.print("IP asignada: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Función requerida por la librería, pero no hace nada si solo publicamos.
  // Si te suscribes a un tópico, la lógica de recepción iría aquí.
}

void reconnect() {
  // Loop hasta que se conecte al broker
  while (!client.connected()) {
    Serial.print("Intentando conectar al broker MQTT...");
    // Intentar conectar con el ID de cliente "ArduinoClient"
    if (client.connect("ArduinoClient")) {
      Serial.println("¡Conectado!");
      
      // Si te suscribes a algún tópico al inicio, hazlo aquí.
      // Ejemplo: client.subscribe("mi_topic_de_recepcion");
      
    } else {
      Serial.print("Error, rc=");
      Serial.print(client.state());
      Serial.println(" — Reintentando en 5 segundos");
      delay(5000);
    }
  }
}

// --- Función de Publicación de Datos (Apartado 4: JSON) ---

/**
 * Simula la lectura de un sensor y lo envía en formato JSON al bróker.
 */
void publishJsonData() {
  // 1. Simulación de lectura de sensor y datos
  float temperature = random(2000, 3000) / 100.0; // Simula Temperatura (20.00°C - 30.00°C)
  float pressure = random(95000, 105000) / 100.0; // Simula Presión (950 hPa - 1050 hPa)

  // 2. Creación del documento JSON
  const size_t capacity = JSON_OBJECT_SIZE(4); // Tamaño suficiente para 4 campos
  StaticJsonDocument<capacity> doc;
  
  doc["device"] = "ESP32S3";
  doc["temp_C"] = temperature;
  doc["pressure_hPa"] = pressure;
  doc["timestamp"] = millis(); // Marca de tiempo del dispositivo

  // 3. Serialización del JSON a una cadena de texto
  char jsonBuffer[capacity * 2]; // Buffer para la cadena serializada
  serializeJson(doc, jsonBuffer);

  // 4. Publicación (QoS 0 por defecto)
  Serial.print("-> Publicando JSON en ");
  Serial.print(json_topic);
  Serial.print(": ");
  Serial.println(jsonBuffer);

  if (client.publish(json_topic, jsonBuffer)) {
    Serial.println("   JSON publicado con éxito.");
  } else {
    Serial.println("   Fallo al publicar JSON.");
  }
}

// --- Función de Publicación de Texto (Apartado 2: Texto) ---

/**
 * Envía un mensaje de texto simple al bróker.
 */
void publishTextData() {
  Serial.print("-> Publicando texto en ");
  Serial.print(text_topic);
  Serial.print(": ");
  Serial.println(text_message);

  // El tercer argumento "false" indica que NO es un mensaje Retained.
  // QoS 0 por defecto.
  if (client.publish(text_topic, text_message)) {
    Serial.println("   Texto publicado con éxito.");
  } else {
    Serial.println("   Fallo al publicar texto.");
  }
}

// --- Setup y Loop Principales ---

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0)); // Inicializa el generador de números aleatorios para la simulación
  
  setup_wifi(); // Conecta a la WiFi (Apartado 1)

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect(); // Asegura la conexión al bróker
  }
  client.loop(); // Mantiene la conexión MQTT y procesa mensajes entrantes

  // Control de tiempo para envío de datos
  static unsigned long lastTextMsg = 0;
  static unsigned long lastJsonMsg = 0;
  unsigned long now = millis();
  
  // Enviar texto cada 5 segundos (Apartado 2)
  if (now - lastTextMsg > 5000) {
    lastTextMsg = now;
    publishTextData();
  }

  // Enviar JSON cada 10 segundos (Apartado 4)
  if (now - lastJsonMsg > 10000) {
    lastJsonMsg = now;
    publishJsonData();
  }
}