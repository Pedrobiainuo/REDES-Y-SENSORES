#include <Adafruit_NeoPixel.h>

// Definición de pines
const int sensorPin = 4;  // Pin analógico para el sensor (ajusta según tu sensor)
const int ledPin = 48;    // Pin para el LED RGB integrado (prueba GPIO 48, 38 o revisa tu placa)

// Configuración del LED RGB (NeoPixel)
#define NUM_LEDS 1  // Solo un LED integrado
Adafruit_NeoPixel led(NUM_LEDS, ledPin, NEO_GRB + NEO_KHZ800);

// Intervalos de tiempo (en milisegundos)
const unsigned long intervaloMedicion = 5000; // 5 segundos
const unsigned long intervaloLed = 1000;     // 1 segundo
unsigned long tiempoAnterior = 0;
bool ledEncendido = false;
unsigned long tiempoLedEncendido = 0;

void setup() {
  // Iniciar comunicación serial
  Serial.begin(115200);
  delay(1000); // Breve espera para estabilizar la conexión serial
  Serial.println("Iniciando programa...");
  Serial.print("Intentando usar LED RGB integrado en GPIO ");
  Serial.println(ledPin);
  
  // Configurar pin del sensor
  pinMode(sensorPin, INPUT);
  
  // Inicializar el LED RGB
  led.begin();
  led.setBrightness(50); // Brillo (0-255, 50 es moderado)
  led.setPixelColor(0, led.Color(0, 0, 0)); // Apagar el LED al inicio
  led.show();
  Serial.println("LED RGB inicializado en estado apagado.");
}

void loop() {
  unsigned long tiempoActual = millis();
  
  // Verificar si han pasado 5 segundos para la medición
  if (tiempoActual - tiempoAnterior >= intervaloMedicion) {
    // Realizar la medición
    int valorSensor = analogRead(sensorPin);
    
    // Mostrar el timestamp y el valor del sensor por el monitor serial
    Serial.print("Timestamp (ms): ");
    Serial.print(tiempoActual);
    Serial.print(" | Valor del sensor: ");
    Serial.println(valorSensor);
    
    // Encender el LED en verde
    led.setPixelColor(0, led.Color(0, 255, 0)); // Color verde (R=0, G=255, B=0)
    led.show();
    Serial.println("LED encendido (verde)");
    ledEncendido = true;
    tiempoLedEncendido = tiempoActual;
    
    // Actualizar el tiempo de la última medición
    tiempoAnterior = tiempoActual;
  }
  
  // Apagar el LED después de 1 segundo
  if (ledEncendido && (tiempoActual - tiempoLedEncendido >= intervaloLed)) {
    led.setPixelColor(0, led.Color(0, 0, 0)); // Apagar el LED
    led.show();
    Serial.println("LED apagado");
    ledEncendido = false;
  }
}
