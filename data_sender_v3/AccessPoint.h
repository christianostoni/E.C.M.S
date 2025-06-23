#include<Arduino.h>
#include <ESP8266WiFi.h>

class AccessPoint{
  public:
    bool initAP();
    bool handleRequest();

    enum deviceType {
      LAVATRICE = 1,
      FRIGORIFERO,
      LAVASTOVIGLIE,
      ASCIUGATRICE,
      CONDIZIONATORE,
      FORNO,
      MICROONDE,
    };

    int selectedDevice;

    String password;  
    String ssid;
};