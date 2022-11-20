import urllib.request
import mysql.connector
import paho.mqtt.client as mqtt


MQTT_ADDRESS = '192.168.137.247'
MQTT_USER = 'shirupi'
MQTT_PASSWORD = 'baby'
MQTT_TOPIC = 'ESPone/+'
humidity = ""
temp=""

def on_connect(client, userdata, flags, rc):
    """ The callback for when the client receives a CONNACK response from the server."""
    print('Connected with result code ' + str(rc))
    client.subscribe(MQTT_TOPIC)


def on_message(client, userdata, msg):
    """The callback for when a PUBLISH message is received from the server."""
    #print(msg.topic + ' ' + str(msg.payload))
    if msg.topic == "ESPone/1":
        temp = msg.payload
    if msg.topic == "ESPone/2":
        humidity = msg.payload
    url = "http://127.0.0.1/iotlab6/Lab6.php?Temperature="+str(temp)+"&Humidity="+str(humidity)
    contents = urllib.request.urlopen(url).read()
    print(contents)


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
    
