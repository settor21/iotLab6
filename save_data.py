import paho.mqtt.client as mqtt
import sqlite3
from time import time

MQTT_HOST = '192.168.0.184'
MQTT_PORT = 8884
MQTT_CLIENT_ID = ''
MQTT_USER = 'Shiru'
MQTT_PASSWORD = 'babybaby'
TOPIC = 'ESPone'

DATABASE_FILE = 'mqtt.db'


def on_connect(mqtt_client, user_data, flags, conn_result):
    mqtt_client.subscribe(TOPIC)


def on_message(mqtt_client, user_data, message):
    payload = message.payload.decode('utf-8')

    db_conn = user_data['db_conn']
    sql = 'INSERT INTO lab6_data (MCU_ID, Light_Intensity, Distance) VALUES (?, ?, ?)'
    cursor = db_conn.cursor()
    cursor.execute(sql, (message.topic, payload, int(time())))
    db_conn.commit()
    cursor.close()


def main():
    db_conn = sqlite3.connect(DATABASE_FILE)
    sql = """
    CREATE TABLE lab6_data (
    ID int(11) NOT NULL,
    MCU_ID int(11) NOT NULL,
    Light_Intensity float NOT NULL,
    Distance float NOT NULL,
    Time timestamp(6) NOT NULL DEFAULT current_timestamp(6) ON UPDATE current_timestamp(6)
)
    """
    cursor = db_conn.cursor()
    cursor.execute(sql)
    cursor.close()

    mqtt_client = mqtt.Client(MQTT_CLIENT_ID)
    mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
    mqtt_client.user_data_set({'db_conn': db_conn})

    mqtt_client.on_connect = on_connect
    mqtt_client.on_message = on_message

    mqtt_client.connect(MQTT_HOST, MQTT_PORT)
    mqtt_client.loop_forever()


main()