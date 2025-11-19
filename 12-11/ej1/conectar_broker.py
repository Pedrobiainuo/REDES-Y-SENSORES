import paho.mqtt.client as mqtt

BROKER = "10.42.0.1"
TOPIC = "test1"

def on_connect(client, userdata, flags, rc):
    print("Conectado:", rc)
    client.subscribe(TOPIC)

def on_message(client, userdata, msg):
    print(f"[{msg.topic}] {msg.payload.decode()}")

client = mqtt.Client("PythonConnector")
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, 1883, 60)
client.loop_start()

client.publish(TOPIC, "Hola desde Python (v1.6.1)")

import time
while True:
    time.sleep(1)
