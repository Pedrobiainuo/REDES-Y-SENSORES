import serial
from pathlib import Path

PUERTO = "COM5"        # ← cámbialo según tu sistema
BAUD   = 115200
ARCHIVO = Path("datos_esp32.csv")

ser = serial.Serial(PUERTO, BAUD, timeout=1)

nuevo = not ARCHIVO.exists()
with ARCHIVO.open("a", encoding="utf-8") as f:
    if nuevo:
        f.write("timestamp_ms,valor\n")

    print(f"Leyendo de {PUERTO} a {BAUD} baudios. Ctrl+C para salir.")
    try:
        while True:
            linea = ser.readline().decode(errors="ignore").strip()
            if linea:
                print("RX:", linea)
                f.write(linea + "\n")
                f.flush()
    except KeyboardInterrupt:
        print("\nDetenido por el usuario.")
    finally:
        ser.close()

print(f"Datos guardados en: {ARCHIVO.resolve()}")
