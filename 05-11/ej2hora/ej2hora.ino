#include <time.h>

struct tm timeinfo;
unsigned long lastPrint = 0;
const long interval = 5000;  // 5 segundos
bool timeSet = false;

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }  // Esperar a que el puerto serie esté listo

  Serial.println("\n=== CONFIGURACIÓN DE FECHA Y HORA ===");
  Serial.println("Ingresa los valores uno por uno:");

  // Solicitar datos al usuario
  int year = pedirEntero("Año (ej. 2025): ", 1900, 2099);
  int yearday = pedirEntero("Día del año (1-366): ", 1, 366);
  int hour = pedirEntero("Hora (0-23): ", 0, 23);
  int minute = pedirEntero("Minuto (0-59): ", 0, 59);
  int second = pedirEntero("Segundo (0-59): ", 0, 59);

  // Configurar la estructura tm
  timeinfo.tm_year = year - 1900;  // tm_year es años desde 1900
  timeinfo.tm_yday = yearday - 1;  // tm_yday es 0-based
  timeinfo.tm_hour = hour;
  timeinfo.tm_min = minute;
  timeinfo.tm_sec = second;
  timeinfo.tm_mday = 1;  // dummy, será calculado
  timeinfo.tm_mon = 0;   // dummy
  timeinfo.tm_wday = 0;  // dummy
  timeinfo.tm_isdst = -1;

  // Convertir a timestamp y configurar el reloj interno
  time_t timestamp = mktime(&timeinfo);
  if (timestamp == -1) {
    Serial.println("Error: Fecha inválida.");
    while (true);
  }

  // Configurar el reloj del sistema
  struct timeval tv = { .tv_sec = timestamp, .tv_usec = 0 };
  settimeofday(&tv, NULL);

  // Forzar actualización de tm con mktime
  localtime_r(&timestamp, &timeinfo);

  Serial.println("¡Fecha y hora configuradas correctamente!\n");
  printCurrentTime();

  timeSet = true;
  lastPrint = millis();
}

void loop() {
  if (!timeSet) return;

  unsigned long now = millis();
  if (now - lastPrint >= interval) {
    lastPrint = now;
    printCurrentTime();
  }
}

// Función para pedir un entero con rango
int pedirEntero(const char* mensaje, int min, int max) {
  int valor;
  while (true) {
    Serial.print(mensaje);
    while (!Serial.available()) {
      delay(100);
    }
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() == 0) {
      Serial.println("Error: entrada vacía.");
      continue;
    }
    valor = input.toInt();
    if (valor >= min && valor <= max) {
      return valor;
    } else {
      Serial.printf("Error: valor fuera de rango [%d-%d]\n", min, max);
    }
  }
}

// Función para imprimir la hora actual
void printCurrentTime() {
  struct timeval tv_now;
  gettimeofday(&tv_now, NULL);
  time_t now = tv_now.tv_sec;
  localtime_r(&now, &timeinfo);

  char buffer[64];
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S (Día del año: %j)", &timeinfo);
  Serial.println(buffer);
}