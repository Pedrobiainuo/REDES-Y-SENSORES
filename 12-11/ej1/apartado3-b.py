import paho.mqtt.client as mqtt

BROKER = "10.42.0.1"
TOPIC = "test1"

def on_connect(client, userdata, flags, rc):
    print("Conectado:", rc)
    client.subscribe(TOPIC)

def on_message(client, userdata, msg):
    print(f"[{msg.topic}] {msg.payload.decode()}")

client = mqtt.Client("PythonPubSub")
client.on_connect = on_connect
client.on_message = on_message

client.connect(BROKER, 1883, 60)
client.loop_start()

while True:
    mensaje = input("Escribe un mensaje para enviar al topic: ")
    client.publish(TOPIC, mensaje)
