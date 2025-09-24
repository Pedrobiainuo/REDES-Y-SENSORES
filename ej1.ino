/*
  Ejercicio 1 - Día 1
  Encender el LED en diferentes colores cada 2 segundos
  ESP32-S3 con LED RGB integrado
*/

void setup() {
  // No hace falta inicialización especial para el RGB
}

void loop() {
#ifdef RGB_BUILTIN
  // Rojo
  rgbLedWrite(RGB_BUILTIN, 255, 0, 0);
  delay(2000);

  // Verde
  rgbLedWrite(RGB_BUILTIN, 0, 255, 0);
  delay(2000);

  // Azul
  rgbLedWrite(RGB_BUILTIN, 0, 0, 255);
  delay(2000);

  // Blanco
  rgbLedWrite(RGB_BUILTIN, 255, 255, 255);
  delay(2000);

  // Apagado
  rgbLedWrite(RGB_BUILTIN, 0, 0, 0);
  delay(2000);
#endif
}
