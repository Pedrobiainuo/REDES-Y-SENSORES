#include <WiFi.h>

// Configura tus pines del LED RGB
const int ledR = 25;  // Rojo
const int ledG = 26;  // Verde
const int ledB = 27;  // Azul

// Datos de WiFi
const char* ssid = "TU_SSID";       // Reemplaza con tu WiFi
const char* password = "TU_PASS";   // Reemplaza con tu contraseña

void setup() {
  Serial.begin(115200);
  
  // Configura pines como salida
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);

  // Apaga todos los colores al inicio
  digitalWrite(ledR, LOW);
  digitalWrite(ledG, LOW);
  digitalWrite(ledB, LOW);

  // Conecta a WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");

  // Mientras no esté conectado
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    // Parpadeo azul
    digitalWrite(ledB, HIGH);
    delay(500);
    digitalWrite(ledB, LOW);
    delay(500);
  }

  // Cuando se conecta, LED verde fijo
  Serial.println("");
  Serial.println("WiFi conectado!");
  digitalWrite(ledG, HIGH);
}

void loop() {
  // Aquí puedes poner tu código del sensor CO2
}
