import paho.mqtt.client as paho
broker="172.16.5.69"
port=1883

def on_publish(client,userdata,result):             
    print("data published \n")
    pass

client1= paho.Client("control1")                           
client1.on_publish = on_publish                         
client1.connect(broker,port)                                 
ret= client1.publish("Wanjiru/pc", "Published message")