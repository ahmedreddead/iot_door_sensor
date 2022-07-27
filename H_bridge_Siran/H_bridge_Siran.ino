

// Set GPIOs for H-bridge 
const int led = 23;
const int gnd = 25;
// M1 POSITIVE 

RTC_DATA_ATTR int bootCount = 0;


#include "Micropolis_Siren.h"


char* ssid = "MICROPOLIS";
char* password = "Micropolis@212";
char* mqtt_server = "10.20.0.31";
int mqtt_port = 1883;

Micropolis_Siren Siren_object(led,gnd);

void setup() {
    Serial.begin(115200);
    //delay(1000);
    //Siren_object.siren_on();
    Siren_object.setup_wifi(ssid, password);
    Siren_object.set_mqtt_server (mqtt_server, mqtt_port);
    reconnect();
    Siren_object.mqtt_publish("Micropolis/Siren", "online");
    

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(500);
  loop_client();

}
