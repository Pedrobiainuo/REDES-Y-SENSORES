# ...existing code...
#pip install paho-mqtt

import paho.mqtt.client as mqtt

BROKER = "10.42.0.1"
PORT = 1883

def on_connect(client, userdata, flags, rc):
    print("Conectado al broker con código:", rc)

client = mqtt.Client("PythonConnector")
client.on_connect = on_connect

client.connect(BROKER, PORT, 60)
client.loop_forever()
# ...existing code...