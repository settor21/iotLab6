#pip install firebase

import firebase_admin
from firebase_admin import credentials
from firebase_admin import firestore
from firebase import firebase

from datetime import date
import time

today = date.today()

cred = credentials.Certificate('iot-lab6-f62c0-f596e59c2461.json')
firebase_admin.initialize_app(cred)
db = firestore.client()
firebase = firebase.FirebaseApplication(
    'https://console.firebase.google.com/u/1/project/iot-lab6-f62c0/firestore/data/~2Flab6_data~2FSensorValues', None)
collection = db.collection("lab6_data")

def pushData():
    i=0
    while i<10:
        res = collection.document("NTest"+str(i)).set({
        'Humidity' : '1',
        'MCU_ID': '2',
        'Temperature': '2',
        'Time':str(today)
        })
        
        print(res)
        time.sleep(3)
        i += 1
    print(1)

pushData()