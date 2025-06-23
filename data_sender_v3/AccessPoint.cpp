#include "AccessPoint.h"
#include "webpage.h"
#include "pgmspace.h"


AccessPoint::deviceType getDeviceType(const String& device) {
  if (device == "lavatrice") return AccessPoint::LAVATRICE;
  else if (device == "frigorifero") return AccessPoint::FRIGORIFERO;
  else if (device == "lavastoviglie") return AccessPoint::LAVASTOVIGLIE;
  else if (device == "asciugatrice") return AccessPoint::ASCIUGATRICE;
  else if (device == "condizionatore") return AccessPoint::CONDIZIONATORE;
  else if (device == "forno") return AccessPoint::FORNO;
  else if (device == "microonde") return AccessPoint::MICROONDE;
  else {
    return AccessPoint::LAVATRICE;  //se non riconosce il tipo di dispositivo, ritorna comunque lavatrice.
  }
}




//inizializzo l'access point configurando SSID del wifi.
bool AccessPoint::initAP() {
  IPAddress deviceIP(192, 168, 1, 2);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.softAPConfig(deviceIP, gateway, subnet);
  if (WiFi.softAP("ECMS_device")) {
    return true;
  } else {
    return false;
  }
}


//funzione che gestisce le richieste e le risposte HTTP
bool AccessPoint::handleRequest() {
  WiFiServer server(80);
  server.begin();
  bool sceltaEffettuata = false;
  String currentLine = "";
  while (!sceltaEffettuata) {
    WiFiClient client = server.accept();
    if (client) {
      Serial.println("c'e un client");
      while (client.connected()) {  //il client risulta connesso anche se la connessione è stata chiusa ma ci sono dei dati non letti
        if (client.available()) {
          char c = client.read();
          if (c == '\n') {                    // Fine di una linea
            if (currentLine.length() == 0) {  // Fine dell'header HTTP
              if (!sceltaEffettuata) {
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println();
                for (int i = 0; i < strlen_P(INDEX_HTML); i++) {
                  client.print((char)pgm_read_byte(&(INDEX_HTML[i])));  // Leggi da PROGMEM
                }
                client.println();
                client.println();
                break;  // Esci dal ciclo di lettura
              } 
              else {
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println();
                for (int i = 0; i < strlen_P(INDEX_HTML); i++) {
                  client.print((char)pgm_read_byte(&(CONFIRM_PAGE[i])));  // Leggi da PROGMEM
                }
                client.println();
                client.println();
                delay(100);
                client.stop();
                WiFi.softAPdisconnect(true);
                Serial.println("invio i dati della pagina di conferma");
                return true;
              }

            } else {  // Fine di una riga, azzera
              currentLine = "";
            }
          } 
          else if (c != '\r') {  // Qualsiasi altro carattere
            currentLine += c;
            if (currentLine.indexOf("device=") != -1 && currentLine.indexOf("ssid=") != -1 && currentLine.indexOf("password=") != -1) {
              int start = currentLine.indexOf("device=") + 7;
              int end = currentLine.indexOf(" ", start - 1);
              AccessPoint::selectedDevice = getDeviceType(currentLine.substring(start, end));

              start = currentLine.indexOf("ssid=") + 5;
              end = currentLine.indexOf("&", start - 1);
              AccessPoint::ssid = currentLine.substring(start, end);

              start = currentLine.indexOf("password=") + 9;
              end = currentLine.indexOf("&", start - 1);
              AccessPoint::password = currentLine.substring(start, end);

              sceltaEffettuata = true;
            }
          }
        }
      }
      client.stop();
      Serial.println("client stop");
    }
    delay(100);
  }
  return false;
}
