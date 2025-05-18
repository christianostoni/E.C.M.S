import paho.mqtt.client as mqtt
import random
import mysql_connect
import json

broker_address = "c4a6477a.ala.eu-central-1.emqxsl.com"
broker_port = 8883
client_id = f'python-mqtt-{random.randint(0, 100)}'
username = 'christian'
password = 'christian123'
topic = "energy_consumption"
mysql = mysql_connect.mysqlConnect()

def on_connect(self,client, userdata, flags, rc):
    if rc == 0:
        print("mqtt_to_mysql.py: Connected to MQTT Broker!")
    else:
        print(f"mqtt_to_mysql.py: Failed to connect, return code {rc}")

def on_message(client, userdata, message):
    print(f"Received message: {message.payload.decode()}")
    payload = json.loads(message.payload.decode())
    macAddress = payload["macAddress"]
    timestamp = payload["timestamp"]
    device = payload["device"]
    data = payload["data"]
    mysql.insertData(timestamp, device, macAddress, data)

client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2,"subscriper_raspberry")
client.tls_set(ca_certs='./../emqxsl-ca.crt')
client.username_pw_set(username, password)
client.on_connect = on_connect

try:
    client.connect(broker_address, broker_port)
    print("mqtt_to_mysql.py: Connection attempt successful")
except Exception as e:
    print(f"mqtt_to_mysql.py:   Error connecting to the broker: {e}")
    
client.subscribe(topic)
client.on_message = on_message
client.loop_forever()


