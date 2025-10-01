/*
  Día 1 - Ejercicio 3
  Mostrar información básica del ESP32-S3 por Serial
*/

#include <WiFi.h>
#include "esp_system.h"
#include "esp_chip_info.h"
#include "esp_mac.h"

void setup() {
  Serial.begin(115200); // AJUSTAR EL BAUD EN EL SERIAL MONITOR A ESTE
  delay(300);

  // --- Info de chip (núcleos, features) ---
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);

  Serial.println("== Informacion basica del ESP32-S3 ==");
  Serial.printf("Nucleos (cores): %d\n", chip_info.cores);

  // --- Modelo y revision ---
  Serial.printf("Modelo de chip: %s\n", ESP.getChipModel());
  Serial.printf("Revision de silicio: %d\n", ESP.getChipRevision());

  // --- Version del SDK (ESP-IDF) ---
  Serial.printf("Version SDK (ESP-IDF): %s\n", ESP.getSdkVersion());

  // --- Memoria Flash ---
  uint32_t flash_bytes = ESP.getFlashChipSize();
  Serial.printf("Tamano de la memoria flash: %u bytes (%.2f MB)\n",
                flash_bytes, flash_bytes / (1024.0 * 1024.0));

  // --- MAC address (robusta: leer del eFuse directamente) ---
  uint8_t mac_sta[6];
  esp_read_mac(mac_sta, ESP_MAC_WIFI_STA);
  Serial.printf("Direccion MAC (WiFi STA): %02X:%02X:%02X:%02X:%02X:%02X\n",
                mac_sta[0], mac_sta[1], mac_sta[2],
                mac_sta[3], mac_sta[4], mac_sta[5]);

  // --- Extra ---
  Serial.printf("CPU freq: %d MHz\n", getCpuFrequencyMhz());
  Serial.println("=====================================\n");
}

void loop() {
  // nada; la info se imprime una vez en setup()
}
