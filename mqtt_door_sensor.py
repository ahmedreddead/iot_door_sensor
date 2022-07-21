import paho.mqtt.client as mqtt
import time

def hub_to_mqtt(st) :
    mqttBroker = "10.20.0.230"
    alarm_bool = False
    topic_sensor_state = "IoT_sensor_state"
    topic_alarm = "IoT_alarm"
    client = mqtt.Client("Hub_")
    client.connect(mqttBroker)
    client.subscribe(topic_sensor_state)
    #preprocess
    serial_number = st.split(",")[0].split("=")[1].strip()
    status = st.split(",")[1].split("=")[1].strip()
    print(serial_number, status)
    if status == "opened":
        status = 1
        alarm_bool = True
    else:
        status = 0
    battery = 3.3
    online = 1
    payload = '\n\"serial_number\":\"{}\"\n , \n\"battery\":{}\n , \n\"state\":{}\n , \n\"online\":{}\n'.format(
        serial_number, battery, status, online)
    payload = '{' + payload + '}' + '\n'
    print(payload)
    client.publish(topic_sensor_state, payload)
    ####### alarm
    if alarm_bool :
        client.subscribe(topic_alarm)
        id = "12F34"
        list_of_sensors = [serial_number]
        payload = '\n\"id\":\"{}\"\n,\n\"sensors\":{}\n'.format(
            id, list_of_sensors)
        payload = '{' + payload + '}' + '\n'
        client.publish(topic_alarm, payload)

def on_message(client, userdata, message):
    print("received message: " ,str(message.payload.decode("utf-8")))
    sensor_status = str(message.payload.decode("utf-8")).strip()

    hub_to_mqtt(sensor_status)

mqttBroker ="10.20.0.230"
client = mqtt.Client("Hub")
client.connect(mqttBroker)
client.loop_start()
client.subscribe("Micropolis/door_sensor/")
client.on_message=on_message
while True :
    time.sleep(10)
#client.loop_stop()