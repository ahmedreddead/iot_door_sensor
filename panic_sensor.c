
#define timeSeconds 5

// Set GPIOs for LED and PIR Motion Sensor
const int led = 18;
const int Push_button = 4;


RTC_DATA_ATTR int bootCount = 0;


#include "ESP_mqtt_wifi.h"
char* ssid = "MICROPOLIS";
char* password = "Micropolis@212";
char* mqtt_server = "10.20.0.31";
int mqtt_port = 1883;


int panicState;
void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(Push_button, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  
  setup_wifi(ssid, password);
  set_mqtt_server (mqtt_server, mqtt_port);
  reconnect();
  
}

void loop() {

  panicState = digitalRead(Push_button);
  Serial.println(panicState);
  if (panicState == HIGH) {
    Serial.println("panicState == HIGH");
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_4,0); //1 = High, 0 = Low
    Serial.println("Going to sleep now");
    delay(1000);
    esp_deep_sleep_start();
    } else {
    Serial.println("panicState == low");
    mqtt_publish("Micropolis/panic_sensor", "Alarm_ON");  
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_4,1); //1 = High, 0 = Low
    Serial.println("Going to sleep now");
    digitalWrite(led, HIGH);
    delay(5000);
    esp_deep_sleep_start();

}
}
