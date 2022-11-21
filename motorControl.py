import urllib.request
import mysql.connector
import paho.mqtt.client as mqtt
import time
import paho.mqtt.publish as publish

MQTT_ADDRESS = '192.168.137.247'
MQTT_USER = 'shirupi'
MQTT_PASSWORD = 'baby'
MQTT_TOPIC = 'Motor1'

MotorPin = LED(2)


def Start_fun():
    if publish(MQTT_TOPIC,"1"):
        print("Start Motor")
        MotorPin.on()
    else:
        print("No action")
        MotorPin.off()
    return


def Stop_fun():
    if publish(MQTT_TOPIC,"0"):
        print("Stop Motor")
        MotorPin.off()
    return


def on_connect(client, userdata, flags, rc):
    """ The callback for when the client receives a CONNACK response from the server."""
    print('Connected with result code ' + str(rc))
    client.subscribe(MQTT_TOPIC)


def on_message(client, userdata, msg):
    """The callback for when a PUBLISH message is received from the server."""
    print(msg.topic + ' ' + str(float(msg.payload)))
    if msg.payload == 1:
        Start_fun()
    else:
        Stop_fun()

def main():
    mqtt_client = mqtt.Client()
    mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
    
    mqtt_client.on_connect = on_connect
    
    mqtt_client.on_message = on_message

    mqtt_client.connect(MQTT_ADDRESS, 1883)
    
    mqtt_client.loop_forever()


if __name__ == '__main__':
    print('MQTT to DB bridge')
    main()
    
