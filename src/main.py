'''
    questo è il file per il server udp. Decodifica i pacchetti udp e invia i dati al broker mqtt
'''
import os
import re
import mqtt_publ
import socket
import sys

def get_ip_address():
    stream = os.popen("hostname -I")
    ip_address = stream.read().strip()
    if ip_address:
        return ip_address
    else:
        raise ValueError ("Nessun IP trovato")
    
ip = get_ip_address()
port = 4444

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_address = (ip, port)
s.bind(server_address)

print(f"Server in ascolto su {ip}:{port}")

mqtt_publisher  =  mqtt_publ.mqtt_publisher()

while True:
    data, address = s.recvfrom(4096)  # Ricevi fino a 4096 byte
    if data:
        print("ho ricevuto un dato")
        # Decodifica il pacchetto (6 byte MAC, 4 byte timestamp, 1 byte valore randomico)
        mac_address = ":".join(f"{b:02x}" for b in data[0:6])  # MAC address
        timestamp = int.from_bytes(data[6:10], byteorder='big')  # Timestamp (big-endian)
        device = data[10]
        currentValue = data[11] / 50  # Valore randomico (1 byte)

        try:
            mqtt_publisher.publish_data(mac_address, timestamp, device, currentValue)
        except Exception as e:
            print(f"Errore durante l'invio dei dati MQTT: {e}")


