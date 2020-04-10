# Simple Python client to show node activity from ttn MQTT brooker with credentials
# Author: R.Schimmel
# www.schimmel-bisolutions.nl
# first install paho.mqtt.client: pip install paho-mqtt
# 
import paho.mqtt.client as mqtt
from myconfig import *

#Call back functions 

# gives connection message
def on_connect(client,userdata,flags,rc):
    print("Connected with result code:"+str(rc))
    # subscribe for all devices of user
    client.subscribe('test')

# gives message from device
def on_message(client,userdata,msg):
    print("Topic",msg.topic + "\nMessage:" + str(msg.payload))
    print(payload2val(msg.payload))

def on_log(client,userdata,level,buf):
    print("message:" + str(buf))
    print("userdata:" + str(userdata))
    
def payload2val(payload):
    return float(payload.partition(b'>')[2])


mqttc= mqtt.Client()
mqttc.on_connect=on_connect
mqttc.on_message=on_message

mqttc.username_pw_set(mqtt_userID,mqtt_password)

# BEWARE, outdated; see the follow up posts to use eu.thethings.network instead
mqttc.connect(mqtt_server)

# and listen to server
run = True
while run:
    mqttc.loop()
