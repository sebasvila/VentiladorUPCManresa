sudo apt install mosquitto mosquitto-clients

On one window, subscribe to a feed named *test*
```
mosquitto_sub -h localhost -t test
```
On the other window, publish something related to *test*
```
mosquitto_pub -h localhost -t test -m 'Hi'
```

A tutorial to [install mosquitto](https://www.digitalocean.com/community/tutorials/how-to-install-and-secure-the-mosquitto-mqtt-messaging-broker-on-ubuntu-18-04).

A nice [tutorial](https://randomnerdtutorials.com/micropython-mqtt-esp32-esp8266/) to use MQTT on ESP32


# Send ADC values through MQTT

# On the ESP

```python
boot.py
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


client_id = ubinascii.hexlify(machine.unique_id())
topic_sub = b'notification'
topic_pub = b'test'

last_message = 0
message_interval = 1
counter = 0

station = network.WLAN(network.STA_IF)

station.active(True)
station.connect(ssid, password)

while station.isconnected() == False:
  pass

print('Connection successful')
print(station.ifconfig())

print('Hi!')
```
And the main file, which is executed after powerup
```python
main.py

def connect():
  global client_id, mqtt_server
  client = MQTTClient(client_id, mqtt_server)
  client.connect()
  print('Connected to %s MQTT broker' % (mqtt_server))
  return client

def restart_and_reconnect():
  print('Failed to connect to MQTT broker. Reconnecting...')
  time.sleep(10)
  machine.reset()

pot=machine.ADC(machine.Pin(33))
pot.width(machine.ADC.WIDTH_10BIT)
pot.atten(machine.ADC.ATTN_11DB)

try:
  client = connect()

except OSError as e:
  restart_and_reconnect()

while True:
  try:
    client.check_msg()
    if (time.time() - last_message) > message_interval:
      msg = b'pot > %d' % pot.read()
      client.publish(topic_pub, msg)
      last_message = time.time()
      counter += 1
  except OSError as e:
    restart_and_reconnect()
```

On the listener window, the pot value is printed every 1s


# Python classes implementing mqtt

[paho.mqtt](http://www.steves-internet-guide.com/into-mqtt-python-client/)

## Test implementation
A script [implementing](https://github.com/PerePala/VentiladorUPCManresa/blob/master/ESP32experiments/mqtt_listener_and_plot.py) a simple mqtt **listener** and plotting the result in real time (Python)

The **sender** is an ESP32, implemented via a [boot.py](https://github.com/PerePala/VentiladorUPCManresa/blob/master/ESP32experiments/boot.py) and a [main.py](https://github.com/PerePala/VentiladorUPCManresa/blob/master/ESP32experiments/main.py) script

TODO: a **sender** script, to allow testing
