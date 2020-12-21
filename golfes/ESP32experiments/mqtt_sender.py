
from time import sleep
from numpy import cos, pi

# mqtt object
import paho.mqtt.client as mqtt

# System credentials
from myconfig import *

####################################### MQTT stuff ##################
#Call back functions 
# gives connection message
def on_connect(client,userdata,flags,rc):
    print("Connected with result code:"+str(rc))
    # subscribe for all devices of user
    client.subscribe('test')

# gives message from device
def on_message(client,userdata,msg):
    global val
    global new_val
    print("Message:" + str(msg.payload))
    val=payload2val(msg.payload)
    new_val=True

def on_log(client,userdata,level,buf):
    print("message:" + str(buf))
    print("userdata:" + str(userdata))
    
def payload2val(payload):
    return 100*float(payload.partition(b'>')[2])/1023


mqttc= mqtt.Client()
mqttc.on_connect=on_connect
mqttc.on_message=on_message

mqttc.username_pw_set(mqtt_userID,mqtt_password)

mqttc.connect(mqtt_server)

# and listen to server

run = True
dt=0.2
t=0
while run:
    mqttc.loop()
    v=500+300*cos(2*pi*t/5);
    mqttc.publish("test","pot > "+str(v))
    sleep(dt)
    t+=dt

