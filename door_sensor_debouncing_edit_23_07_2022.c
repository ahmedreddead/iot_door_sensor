

#include <WiFi.h>
#include <PubSubClient.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#define DOOR_SENSOR_PIN 4
#define Hall_Sensor 35

RTC_DATA_ATTR int bootCount = 0;
float doorState,MagniticField;

const char* ssid = "MICROPOLIS";
const char* password = "Micropolis@212";

const char* mqtt_server = "10.20.0.230";

char* msgTopic = "Micropolis/door_sensor/";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
char* last_state = "Non";

int normal_counter = 0;
int high_counter = 0;
int low_counter = 0;

 
void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  pinMode(DOOR_SENSOR_PIN , INPUT_PULLUP);

}

void setup_wifi() {
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

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  else {

  normal_counter++ ;
  doorState = digitalRead(DOOR_SENSOR_PIN);
  MagniticField = hallRead();
  Serial.println(doorState);
  if (doorState == HIGH) {
    Serial.println("the door is open ");
    high_counter++ ;
    
    if (last_state != "open")
{
    client.publish(msgTopic, "id = 1, status = opened");
    last_state = "open"; 
}

    
    } else {
    Serial.println("the door is close ");
    low_counter++ ;
    if (last_state != "close")
{
    client.publish(msgTopic, "id = 1 , status = closed"); 
    last_state = "close"; 

    
}
    
    }

  if (normal_counter == 30)
  {
    if (high_counter == normal_counter && low_counter == 0 ) 
    {
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_4,0); //1 = High, 0 = Low
    Serial.println("Going to sleep now");
    delay(1000);
    esp_deep_sleep_start();
      }

    else if ( low_counter == normal_counter && high_counter == 0 )
    {
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_4,1); //1 = High, 0 = Low
    Serial.println("Going to sleep now");
    delay(1000);
    esp_deep_sleep_start();
      
      }

    else 

    {
      normal_counter = 0;
      high_counter = 0;
      low_counter = 0;
      
      }
    
    }
  //client.loop();


}
delay(1000);
}
