import paho.mqtt.publish as publish
import paho.mqtt.client as mqtt
from gpiozero import LED

MQTT_SERVER = '192.168.137.247'
MQTT_USER = 'shirupi'
MQTT_PASSWORD = 'baby'
MQTT_PORT = 1883


Motor_State= 'Motor/state'



publish.single(Motor_State,"OFF",hostname = MQTT_ADDRESS)
publish.single(Motor_State,"ON",hostname = MQTT_ADDRESS)


# LED_Pin = LED(13)


# def Start_fun(Start):
#     if publish(Start,"1"):
#         print("Start Motor")
#         LED_Pin.on()
#     else:
#         print("No action")
#         LED_Pin.off()
#     return


# def Stop_fun(Stop):
#     if publish(Stop,"0"):
#         print("Stop Motor")
#         LED_Pin.off()
#     return


# def on_publish(client,userdata,result):
#        Start_fun()
#        Stop_fun()
#     print("Published \n")
    



# # def on_connect(client, userdata, flags, rc):
# #     """ The callback for when the client receives a CONNACK response from the server."""
# #     print('Connected with result code ' + str(rc))
# #     # client.publish(MQTT_TOPIC)
# #     Start_fun()
# #     Stop_fun()


# def on_message(client, userdata, msg):
#     """The callback for when a PUBLISH message is received from the server."""
#     print(msg.topic + ' ' + str(msg.payload))
#     if msg.topic == "Motor/1":
#        Start_fun()
#     if msg.topic == "Motor/2":
#        Stop_fun()


#     # client.publish(MQTT_TOPIC1,val)

# # def pushData():
    
# #     mydb = mysql.connector.connect(
# #         host="localhost",
# #         user="root",
# #         password="",
# #         database="sensor_data"
# #     )
# #     mycursor = mydb.cursor()
# #     sql = "INSERT INTO lab6_data (MCU_ID,Light_Intensity,Distance) VALUES (%s,%s,%s)"
# #     val = ("1", temp, humidity)
# #     mycursor.execute(sql, val)
# #     mydb.commit()
# #     print(mycursor.rowcount, "record inserted.")

# def main():
#     mqtt_client = mqtt.Client()
#     mqtt_client.username_pw_set(MQTT_USER, MQTT_PASSWORD)
    
#     mqtt_client.on_publish = on_publish
    
#     mqtt_client.connect(MQTT_ADDRESS, 1883)
    
#     mqtt_client.loop_forever()
#         # pushData()


# if __name__ == '__main__':
#     print('MQTT to DB bridge')
#     main()
    
