#include "WiFi.h"

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Escaneando redes WiFi...");

  // Modo estación (no conecta a ninguna red)
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  int n = WiFi.scanNetworks();
  Serial.println("Escaneo completado.");
  if (n == 0) {
    Serial.println("No se encontraron redes.");
  } else {
    Serial.println("Redes encontradas:");
    for (int i = 0; i < n; ++i) {
      // Imprime SSID, RSSI y si está oculta
      Serial.printf("%d: %s (%d dBm) %s\n", i + 1,
                    WiFi.SSID(i).c_str(),
                    WiFi.RSSI(i),
                    (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Abierta" : "Segura");
      delay(10);
    }
  }
}

void loop() {
  // No se repite el escaneo en loop
}
