import paho.mqtt.client as mqtt
import time

BROKER = "10.42.0.1"
PORT = 1883
TOPIC = "test1"

# Callback cuando nos conectamos al broker
def on_connect(client, userdata, flags, rc):
    print("Conectado al broker con código:", rc)
    client.subscribe(TOPIC)
    print(f"Suscrito al topic: {TOPIC}")

# Callback cuando llega un mensaje
def on_message(client, userdata, msg):
    print(f"[Mensaje recibido] {msg.topic}: {msg.payload.decode()}")

# Crear cliente MQTT
client = mqtt.Client("PythonPubSub")

# Asociar callbacks
client.on_connect = on_connect
client.on_message = on_message

# Conectar
print("Conectando al broker MQTT...")
client.connect(BROKER, PORT, 60)

# Iniciar la recepción en segundo plano
client.loop_start()

# ---- Enviar mensajes al topic ----
print("Escribe mensajes para enviar al topic 'test1'.")
print("Pulsa CTRL+C para salir.")

try:
    while True:
        mensaje = input("> ")
        client.publish(TOPIC, mensaje)
        print(f"[Enviado] {mensaje}")

except KeyboardInterrupt:
    print("\nCerrando conexión...")

client.loop_stop()
client.disconnect()
