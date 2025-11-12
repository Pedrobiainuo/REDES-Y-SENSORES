#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "LunarComms4";
const char* password = "11223344";
const char* mqtt_server = "10.42.0.1";
const int mqtt_port = 1883;
const char* topic = "test1";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Mensaje recibido en el topic: ");
  Serial.println(topic);
  Serial.print("Contenido: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
  }
  Serial.println();
}

void setup_wifi() {
  Serial.println();
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi ");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectada.");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conectar al broker...");
    if (client.connect("ArduinoClient")) {
      Serial.println("Conectado al broker MQTT.");
      client.subscribe(topic);
    } else {
      Serial.print("Error rc=");
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
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Enviar mensaje cada 5 segundos
  static unsigned long lastMsg = 0;
  unsigned long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    String message = "Hola desde Arduino!";
    client.publish(topic, message.c_str());
    Serial.print("Mensaje enviado: ");
    Serial.println(message);
  }
}
