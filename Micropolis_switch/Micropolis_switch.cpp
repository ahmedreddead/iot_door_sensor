
#include "Micropolis_switch.h"

#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);

Micropolis_switch object(26,27);
// Date constructor
Micropolis_switch::Micropolis_switch(int vcc, int gnd)
{
    Set_volt(vcc,gnd);
}
// Date member function
void Micropolis_switch::Set_volt(int vcc0, int gnd0)
{
    vcc = vcc0;
    gnd = gnd0;
    status = "off"
    pinMode(vcc, OUTPUT);
    pinMode(gnd, OUTPUT);
    digitalWrite(this->vcc, LOW);
    digitalWrite(this->gnd, LOW);  
}

void Micropolis_switch::switch_on()
{
 digitalWrite(this->vcc, HIGH);
 digitalWrite(this->gnd, LOW);
 status = "on"
 client.publish("Micropolis/switch","on")
}


void Micropolis_switch::switch_off()
{
 digitalWrite(this->vcc, LOW);
 digitalWrite(this->gnd, LOW);
 status = "off"
 client.publish("Micropolis/switch","off")
}



void Micropolis_switch::set_mqtt_server(char *mqtt_server, int port)
{
   client.setServer(mqtt_server, port);
   object = *this;
   client.setCallback(callback);
   client.publish("Micropolis/switch","off")
}

void Micropolis_switch::mqtt_publish (char *topic, char *message)
{
   client.publish(topic, message);
   
}

void Micropolis_switch::setup_wifi(char *ssid ,char *password ) {
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
    if (client.connect("MicroPolis_switch")) {
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

  if (String(topic) == "Micropolis/switch") {
    if(messageTemp == "on"){

      object.switch_on();
    }
    if(messageTemp == "off"){
      object.switch_off();
    }

  }
}

void loop_client ()
{  if (!client.connected()) {
    reconnect();
  }
    client.loop();
}