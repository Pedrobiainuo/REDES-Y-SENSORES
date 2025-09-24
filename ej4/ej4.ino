// Ejercicio 4 - Parte A: enviar número aleatorio cada 2 s
// Placa: ESP32-S3

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }   // Espera a que el USB esté listo (S3)
  delay(300);

  // Semilla simple para el RNG (no crítica para el ejercicio)
  randomSeed(micros());

  Serial.println("ESP32-S3 listo. Enviando numeros...");
}

void loop() {
  int valor = random(0, 100);           // 0..99
  // Mando también el tiempo (ms) para que quede bonito en el log
  Serial.printf("%lu,%d\n", millis(), valor);
  delay(2000);
}
