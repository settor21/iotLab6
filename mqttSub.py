import mysql.connector
import paho.mqtt.client as mqtt

MQTT_ADDRESS = '192.168.137.247'
MQTT_USER = 'shiru.py'
MQTT_PASSWORD = 'baby'
MQTT_TOPIC = 'ESPone/+'
humidity = ""
temp=""

def on_connect(client, userdata, flags, rc):
    """ The callback for when the client receives a CONNACK response from the server."""
    print('Connected with result code ' + str(rc))
    client.subscribe(MQTT_TOPIC)


def on_message_Temp(client, userdata, msg):
    """The callback for when a PUBLISH message is received from the server."""
    #print(msg.topic + ' ' + str(msg.payload))
    if msg.topic == "ESPone/1":
        temp = msg.payload
    
def on_message_Humidity(client, userdata, msg):
    """The callback for when a PUBLISH message is received from the server."""
    #print(msg.topic + ' ' + str(msg.payload))
    if msg.topic == "ESPone/2":
        humidity = msg.payload

def pushData():
    
    mydb = mysql.connector.connect(
        host="localhost",
        user="root",
        password="",
        database="sensor_data"
    )
    mycursor = mydb.cursor()
    sql = "INSERT INTO lab6_data (MCU_ID,Light_Intensity,Distance) VALUES (%s,%s,%s)"
    val = ("1", temp, humidity)
    mycursor.execute(sql, val)
    mydb.commit()
    print(mycursor.rowcount, "record inserted.")

def main():
    mqtt_client = mqtt.Client()
    mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
    
    mqtt_client.on_connect = on_connect
    
    mqtt_client.on_message_Humidity = on_message_Humidity
    mqtt_client.on_message_Temp = on_message_Temp

    mqtt_client.connect(MQTT_ADDRESS, 1883)
    
    while(1):
        mqtt_client.loop_forever()
        pushData()


if __name__ == '__main__':
    print('MQTT to DB bridge')
    main()
    
