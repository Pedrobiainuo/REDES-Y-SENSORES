import paho.mqtt.client as mqtt

BROKER = "10.42.0.1"
TOPIC = "test1"

# Callback cuando se conecta al broker
def on_connect(client, userdata, flags, rc):
    print("Conectado al broker con código:", rc)
    client.subscribe(TOPIC)
    print(f"Suscrito al topic: {TOPIC}")

# Callback cuando se recibe un mensaje
def on_message(client, userdata, msg):
    print(f"Mensaje recibido en {msg.topic}: {msg.payload.decode()}")

# Crear cliente MQTT (simple en paho-mqtt 1.6.1)
client = mqtt.Client("PythonSubscriber")

client.on_connect = on_connect
client.on_message = on_message

# Conectar al broker
print("Conectando al broker MQTT...")
client.connect(BROKER, 1883, 60)

# Bucle infinito para escuchar los mensajes
client.loop_forever()
