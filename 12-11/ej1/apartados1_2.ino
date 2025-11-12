#include <WiFi.h>          // Usa <ESP8266WiFi.h> si es un ESP8266
#include <PubSubClient.h>

const char* ssid = "LunarComms4";
const char* password = "11223344";
const char* mqtt_server = "10.42.0.1";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

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

void reconnect() {
  // Loop hasta que se conecte al broker
  while (!client.connected()) {
    Serial.print("Intentando conectar al broker MQTT...");
    if (client.connect("ArduinoClient")) {
      Serial.println("¡Conectado!");
    } else {
      Serial.print("Error, rc=");
      Serial.print(client.state());
      Serial.println(" — Reintentando en 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
