
#include "Micropolis_Siren.h"

#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

Micropolis_Siren object(4,5);
// Date constructor
Micropolis_Siren::Micropolis_Siren(int vcc, int gnd)
{
    Set_volt(vcc,gnd);
}
// Date member function
void Micropolis_Siren::Set_volt(int vcc0, int gnd0)
{
    vcc = vcc0;
    gnd = gnd0;
    pinMode(vcc, OUTPUT);
    pinMode(gnd, OUTPUT);  
}

void Micropolis_Siren::siren_on()
{
 digitalWrite(this->vcc, HIGH);
 digitalWrite(this->gnd, LOW);
}


void Micropolis_Siren::siren_off()
{
 digitalWrite(this->vcc, LOW);
 digitalWrite(this->gnd, LOW);
}



void Micropolis_Siren::set_mqtt_server(char *mqtt_server, int port)
{
   client.setServer(mqtt_server, port);
   object = *this;
   client.setCallback(callback);
}

void Micropolis_Siren::mqtt_publish (char *topic, char *message)
{
   client.publish(topic, message);
}

void Micropolis_Siren::setup_wifi(char *ssid ,char *password ) {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  Serial.printf("pass the begin");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("MicroPolis_Siren")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("Micropolis/panic_sensor");
      client.subscribe("Micropolis/hub");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}





 void callback(char* topic, byte* message, unsigned int length ) {

  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  if (String(topic) == "Micropolis/panic_sensor") {
    if(messageTemp == "Alarm_ON"){
      Serial.println("on");
      object.siren_on();
      delay(10000);
      object.siren_off();
    }

  }
  if (String(topic) == "Micropolis/hub") {
    if(messageTemp == "Alarm_ON"){
      Serial.println("on");
      object.siren_on();
      delay(10000);
      object.siren_off();
    }

  }
}

void loop_client ()
{  if (!client.connected()) {
    reconnect();
  }
    client.loop();
}