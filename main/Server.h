#include <ESP8266WiFi.h>
#include "Additional.h"

#ifndef SERVER_H
#define SERVER_H

class ConfigServer : public WiFiServer{
  private:

  //WiFiServer server(80);
  //store HTTP request
  String header;

  // Auxiliar variables to store the current output state
  String output5State = "off";
  String output4State = "off";

  // Assign output variables to GPIO pins
  const int output5 = 5;
  const int output4 = 4;

  // Current time
  unsigned long currentMillisTime = millis();
  // Previous time
  unsigned long previousTime = 0; 
  // Define timeout time in milliseconds (example: 2000ms = 2s)
  const long timeoutTime = 2000;
  
  public:
  void handleClients();
  explicit ConfigServer(int x) : WiFiServer(x) { }
};

#endif
