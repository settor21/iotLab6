import paho.mqtt.publish as publish
MQTT_ADDRESS = '192.168.137.247'
MQTT_PATH = "Motor1"
while 1:
    publish.single(MQTT_PATH, "0", hostname=MQTT_ADDRESS)