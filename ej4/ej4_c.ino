/*
  Ejercicio 4 - Apartado C
  Enviar un contador por Serial cada 2 segundos (ESP32-S3)
  Salida en formato: <timestamp_ms>,<contador>
  Ejemplo: 2005,0
           4006,1
*/

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }  // Espera a que el USB esté listo (útil en S3)
  delay(300);

  Serial.println("ESP32-S3 listo. Enviando contador cada 2 s...");
}

long contador = 0;

void loop() {
  // Enviamos: tiempo desde arranque (ms) y el contador
  Serial.printf("%lu,%ld\n", millis(), contador);
  contador++;
  delay(2000);  // 2 segundos
}

