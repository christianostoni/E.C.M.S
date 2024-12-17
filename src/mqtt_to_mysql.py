import paho.mqtt.client as mqtt
import time
import random

broker_address = "xaa9b307.ala.us-east-1.emqxsl.com"
broker_port = 8883
client_id = f'python-mqtt-{random.randint(0, 100)}'
username = 'christian'
password = 'christian123'
topic = "energy_consumption"

def on_connect(self,client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
    else:
        print(f"Failed to connect, return code {rc}")

def on_message(client, userdata, message):
    print(f"Received message: {message.payload.decode()}")

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2,"subscriper_raspberry")
client.tls_set(ca_certs='./../emqxsl-ca.crt')
client.username_pw_set(username, password)
client.on_connect = on_connect

try:
    client.connect(broker_address, broker_port)
    print("Connection attempt successful")
except Exception as e:
    print(f"Error connecting to the broker: {e}")
    
client.subscribe(topic)
client.on_message = on_message
client.loop_forever()


