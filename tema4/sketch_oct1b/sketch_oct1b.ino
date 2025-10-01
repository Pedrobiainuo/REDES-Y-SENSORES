#include <Wire.h>

// Dirección I2C del SGP30
#define SGP30_I2C_ADDRESS 0x58

// Pines I2C personalizados
#define SDA_PIN 4  // Cambia según los pines disponibles
#define SCL_PIN 5  // Cambia según los pines disponibles

// Comandos del SGP30
const uint8_t CMD_IAQ_INIT[] = {0x20, 0x03};
const uint8_t CMD_MEASURE_IAQ[] = {0x20, 0x08};

// Definiciones de pines
const int ledPin = 38; // Pin del LED integrado (ajusta según tu placa, ej. GPIO 38 para ESP32-S3-DevKitC-1)

// Variables para el control de tiempo
unsigned long previousMillis = 0;
const long interval = 5000;
const long ledOnTime = 1000;
bool ledState = false;
unsigned long ledTurnOnMillis = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // Apagar LED (HIGH si el LED es activo bajo, común en LEDs integrados)
  
  // Inicializar I2C con pines personalizados
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(100000); // Frecuencia I2C a 100kHz
  
  Serial.println("Iniciando escaneo I2C...");
  bool found = false;
  Wire.beginTransmission(SGP30_I2C_ADDRESS);
  int result = Wire.endTransmission();
  if (result == 0) {
    Serial.println("SGP30 encontrado en 0x58");
    found = true;
  } else {
    Serial.print("SGP30 no encontrado en 0x58. Código de error: ");
    Serial.println(result);
  }
  
  if (found && sgp30_init()) {
    Serial.println("Sensor SGP30 inicializado correctamente");
  } else {
    Serial.println("No se pudo inicializar el SGP30, verifica las conexiones!");
    while (1);
  }
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    digitalWrite(ledPin, LOW); // Encender LED (LOW si es activo bajo)
    ledState = true;
    ledTurnOnMillis = currentMillis;
    
    uint16_t tvoc, eco2;
    if (sgp30_measure(&tvoc, &eco2)) {
      Serial.print("TVOC: ");
      Serial.print(tvoc);
      Serial.print(" ppb\t");
      Serial.print("eCO2: ");
      Serial.print(eco2);
      Serial.println(" ppm");
    } else {
      Serial.println("Error al medir con el SGP30");
    }
  }
  
  if (ledState && (currentMillis - ledTurnOnMillis >= ledOnTime)) {
    digitalWrite(ledPin, HIGH); // Apagar LED (HIGH si es activo bajo)
    ledState = false;
  }
}

bool sgp30_init() {
  Wire.beginTransmission(SGP30_I2C_ADDRESS);
  Wire.write(CMD_IAQ_INIT, sizeof(CMD_IAQ_INIT));
  int result = Wire.endTransmission();
  if (result != 0) {
    Serial.print("Error en inicialización I2C, código: ");
    Serial.println(result);
    return false;
  }
  delay(10);
  return true;
}

bool sgp30_measure(uint16_t* tvoc, uint16_t* eco2) {
  Wire.beginTransmission(SGP30_I2C_ADDRESS);
  Wire.write(CMD_MEASURE_IAQ, sizeof(CMD_MEASURE_IAQ));
  int result = Wire.endTransmission();
  if (result != 0) {
    Serial.print("Error al enviar comando de medición, código: ");
    Serial.println(result);
    return false;
  }
  
  delay(12);
  
  Wire.requestFrom(SGP30_I2C_ADDRESS, 6);
  if (Wire.available() != 6) {
    Serial.print("Error: se esperaban 6 bytes, recibidos: ");
    Serial.println(Wire.available());
    return false;
  }
  
  uint8_t eco2_high = Wire.read();
  uint8_t eco2_low = Wire.read();
  Wire.read(); // Ignorar CRC
  *eco2 = (eco2_high << 8) | eco2_low;
  
  uint8_t tvoc_high = Wire.read();
  uint8_t tvoc_low = Wire.read();
  Wire.read(); // Ignorar CRC
  *tvoc = (tvoc_high << 8) | tvoc_low;
  
  return true;
}