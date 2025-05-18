import paho.mqtt.client as mqtt
import json
import random
class mqtt_publisher:
    def __init__(self):
        broker_address = "c4a6477a.ala.eu-central-1.emqxsl.com"
        broker_port = 8883
        username = "christian"
        password = "christian123"
        client_id = f'python-mqtt-{random.randint(0, 100)}'
        self.topic = "energy_consumption"
        def on_connect(self,client:mqtt, userdata, flags, rc):
            print(type(rc))
            print(rc)
            if rc == 0:
                print("Connected to MQTT Broker!")
            else:
                print(f"Failed to connect, return code {rc}")

        self.client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2,client_id)
        self.client.tls_set(ca_certs='./../emqxsl-ca.crt')
        self.client.username_pw_set(username, password)
        self.client.on_connect = on_connect

        try:
            print("test1")
            self.client.connect(broker_address, broker_port)
            print("Connection attempt successful")
            self.client.loop_start()
        except Exception as e:
            print(f"Error connecting to the broker: {e}")


    def publish_data(self, macAddress, timestamp, device, data):
        payload = { "macAddress":macAddress,
                    "timestamp":timestamp,
                    "device":device,
                    "data":data}
        
        payload_json = json.dumps(payload)
        result = self.client.publish(self.topic, payload_json)
        status = result[0]
        if status == 0:
            print(f"Data published successfully: {payload_json}")
        else:
            print("Data not published")