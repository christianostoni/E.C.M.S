//revisione 3: esp8266
/*
revisione 3:  arduino fa da access point per selezionare la connessione alla quale connettersi e 
              trova automaticamente il raspberry pi sulla rete con il protocollo multicast DNS
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include "AccessPoint.h"
#include <mDNSResolver.h>
#include "ACS712.h"

AccessPoint accessPoint;
WiFiUDP udp;
NTPClient timeClient(udp, "pool.ntp.org", 0, 60000);
byte macAddress[6];
ACS712  ACS(A0, 3.3, 1023, 100);

#define UDP_PORT 4444
IPAddress host = INADDR_NONE;
/**
Buffer per il pacchetto UDP
6byte ---> indirizzo mac
4byte ---> timestamp
1byte ---> selected device
1byte --->  sensor data
*/
char packetBuffer[12];

//namespace per la libreria del resolver dns(trovare indirizzo ip raspberry)
using namespace mDNSResolver;
Resolver resolver(udp);

void setup() {
  Serial.begin(9600);

  ACS.autoMidPoint();

  if (accessPoint.initAP()) {
    Serial.println("ap inizializzato correttamente");
  } else {
    Serial.println("ap non inizializzato");
    while (true)
      ;
  }
  delay(500);

  if (accessPoint.handleRequest()) {
    Serial.println(accessPoint.password);
    formatData(accessPoint.password);
    Serial.println(accessPoint.password);
    WiFi.begin(accessPoint.ssid, accessPoint.password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connessione Wi-Fi in corso...");
      Serial.println(accessPoint.password);
      Serial.println(accessPoint.ssid);
    }
    Serial.println("Connesso a Wi-Fi!");
  }
  else{
    Serial.println("errore durante l'access point");
    while(true);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connessione Wi-Fi in corso...");
  }
  Serial.println("Connesso a Wi-Fi!");

  WiFi.macAddress(macAddress);
  Serial.print("Indirizzo IP Arduino: ");
  Serial.println(WiFi.localIP());

  // Inizializza NTP Client
  timeClient.begin();

  while (host == INADDR_NONE) {
    host = getRaspIP();
  }
  Serial.print("raspberry trovato: ");
  Serial.println(host);
}

void loop() {
  // Esegui prima l'aggiornamento NTP
  timeClient.update();
  Serial.print("Timestamp aggiornato: ");
  Serial.println(timeClient.getEpochTime());

  // Leggi corrente dal sensore ACS712
  float ampere = ACS.mA_AC_sampling() / 1000.0; // Converti da mA a A
  Serial.print("Corrente letta: ");
  Serial.print(ampere, 3); // stampo con 3 decimali
  Serial.println(" A");

  // Scala il valore float per comprimerlo in 1 byte
  uint8_t value = (uint8_t)(ampere * 50.0);  // esempio: 0.20A -> 10

  // Prepara il pacchetto UDP
  int timestamp = timeClient.getEpochTime();

  memcpy(packetBuffer, macAddress, sizeof(macAddress));
  packetBuffer[6] = (timestamp >> 24) & 0xFF;
  packetBuffer[7] = (timestamp >> 16) & 0xFF;
  packetBuffer[8] = (timestamp >> 8) & 0xFF;
  packetBuffer[9] = timestamp & 0xFF;
  packetBuffer[10] = accessPoint.selectedDevice;
  packetBuffer[11] = value;

  if (!udp.beginPacket(host, UDP_PORT)) {
    Serial.println("Errore: indirizzo IP del Raspberry non raggiungibile");
    delay(1000);
    return;
  }

  udp.write((uint8_t *)packetBuffer, sizeof(packetBuffer));  // invia 12 byte

  if (!udp.endPacket()) {
    Serial.println("Errore: pacchetto non inviato");
  } else {
    Serial.println("Pacchetto inviato con successo!");
  }

  delay(10000);  // Pausa di 10 secondi
}



IPAddress getRaspIP() {
  resolver.setLocalIP(WiFi.localIP());
  IPAddress ip = resolver.search("pi.local");
  return ip;
}

void formatData(String &data) {
  int index = data.indexOf('%');
  if (index == -1) { return; }
  String code = "";
  code += data.charAt(index + 1);
  code += data.charAt(index + 2);
  if (code == "23") {
    data.replace("%23", "#");
  }
}
