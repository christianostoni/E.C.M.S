'''
    questo è il file per il server udp. Decodifica i pacchetti udp e invia i dati al broker mqtt
'''
import socket
import mqtt_publ

ip = "127.0.0.1"
port = 4444

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_address = (ip, port)
s.bind(server_address)

print(f"Server in ascolto su {ip}:{port}")

mqtt_publisher  =  mqtt_publ.mqtt_publisher()

while True:
    data, address = s.recvfrom(4096)  # Ricevi fino a 4096 byte
    if data:
        # Decodifica il pacchetto (6 byte MAC, 4 byte timestamp, 1 byte valore randomico)
        mac_address = ":".join(f"{b:02x}" for b in data[0:6])  # MAC address
        timestamp = int.from_bytes(data[6:10], byteorder='big')  # Timestamp (big-endian)
        device = data[10]
        random_value = data[11]  # Valore randomico (1 byte)

        try:
            mqtt_publisher.publish_data(mac_address, timestamp, device, random_value)
        except Exception as e:
            print(f"Errore durante l'invio dei dati MQTT: {e}")