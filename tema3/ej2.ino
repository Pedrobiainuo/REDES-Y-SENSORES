/*
  Ejercicio 2 - Día 1
  Fade In / Fade Out en blanco con el LED RGB del ESP32-S3
*/

void setup() {
  // No hace falta inicializar el LED RGB
}

void loop() {
#ifdef RGB_BUILTIN
  // --- Aparición (fade in) ---
  for (int brillo = 0; brillo <= 255; brillo++) {
    rgbLedWrite(RGB_BUILTIN, brillo, brillo, brillo);  // Blanco con intensidad "brillo"
    delay(10);  // velocidad del fade (10 ms por paso)
  }

  // --- Desvanecimiento (fade out) ---
  for (int brillo = 255; brillo >= 0; brillo--) {
    rgbLedWrite(RGB_BUILTIN, brillo, brillo, brillo);
    delay(10);
  }

  // Pausa antes de repetir el ciclo
  delay(500);
#endif
}
