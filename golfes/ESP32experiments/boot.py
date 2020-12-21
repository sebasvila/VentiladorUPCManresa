# Complete project details at https://RandomNerdTutorials.com

import time
from umqttsimple import MQTTClient
import ubinascii
import machine
import micropython
import network
import esp
esp.osdebug(None)
import gc
gc.collect()

from myconfig import *
#  ssid = 'REPLACE_WITH_YOUR_SSID'
#  password = 'REPLACE_WITH_YOUR_PASSWORD'
#  mqtt_server = 'REPLACE_WITH_YOUR_MQTT_BROKER_IP'
#  mqtt_userID = 'user'
#  mqtt_password = 'password'
#  mqtt_port = 0



station = network.WLAN(network.STA_IF)

station.active(True)
station.connect(ssid, password)

while station.isconnected() == False:
  pass

print('Connection successful')
print(station.ifconfig())

print('Hi!')

