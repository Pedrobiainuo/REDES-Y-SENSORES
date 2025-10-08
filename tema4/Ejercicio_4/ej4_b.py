import serial
import matplotlib.pyplot as plt
from collections import deque

# Ajusta el puerto según tu sistema (en Windows suele ser COM3, COM4, etc.)
puerto = "COM5"
baudrate = 115200

ser = serial.Serial(puerto, baudrate, timeout=1)

# Buffers de datos (últimos 100 puntos)
co2_data = deque(maxlen=100)
tvoc_data = deque(maxlen=100)
tiempo = deque(maxlen=100)

plt.ion()
fig, ax = plt.subplots()
line_co2, = ax.plot([], [], label="CO₂ (ppm)")
line_tvoc, = ax.plot([], [], label="TVOC (ppb)")
ax.legend()
ax.set_xlabel("Muestras (cada 5 s)")
ax.set_ylabel("Valor medio")
ax.set_title("Lecturas del SGP30")

contador = 0

try:
    while True:
        linea = ser.readline().decode("utf-8").strip()
        if linea.startswith("DATA,"):
            _, co2_str, tvoc_str = linea.split(",")
            co2 = float(co2_str)
            tvoc = float(tvoc_str)
            contador += 1

            co2_data.append(co2)
            tvoc_data.append(tvoc)
            tiempo.append(contador)

            line_co2.set_data(tiempo, co2_data)
            line_tvoc.set_data(tiempo, tvoc_data)
            ax.relim()
            ax.autoscale_view()
            plt.pause(0.01)

except KeyboardInterrupt:
    print("Finalizado por el usuario.")
finally:
    ser.close()
