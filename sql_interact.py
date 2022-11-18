
import mysql.connector

mydb = mysql.connector.connect(
  host="localhost",
  user="root",
  password="",
  database="sensor_data"
)

topic = ""
mycursor = mydb.cursor()

sql = "INSERT INTO lab6_data (MCU_ID,Light_Intensity,Distance) VALUES (%s,%s,%s)"
if topic == "SE":
    val = ("1", "1", "1") #esp1 dummy data, variables will be subscribed from topic
else: 
    val = ("2", "1", "1")  # id 2
    
mycursor.execute(sql, val)

mydb.commit()

print(mycursor.rowcount, "record inserted.")

