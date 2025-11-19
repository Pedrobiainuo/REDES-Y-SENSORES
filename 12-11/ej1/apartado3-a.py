from paho.mqtt.client import Client, CallbackAPIVersion

BROKER = "10.42.0.1"
PORT = 1883
TOPIC = "test1"

# ---- CALLBACKS CORRECTOS PARA PAHO-MQTT 2.x ----

def on_connect(client, userdata, flags, reason_code, properties):
    print("Conectado al broker. Código:", reason_code)
    client.subscribe(TOPIC)
    print(f"Suscrito al topic: {TOPIC}")

def on_message(client, userdata, msg):
    print(f"Mensaje recibido en {msg.topic}: {msg.payload.decode()}")

# ---- CLIENTE ----

client = Client(
    client_id="PythonSubscriber",
    callback_api_version=CallbackAPIVersion.VERSION1
)

client.on_connect = on_connect
client.on_message = on_message

print("Conectando al broker MQTT...")
client.connect(BROKER, PORT)

client.loop_forever()
