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
Added a sender that sends mqtt packets, [mqtt_sender.py](https://github.com/PerePala/VentiladorUPCManresa/blob/master/ESP32experiments/mqtt_sender.py)

UPDATE: A **sender script** is [here](https://github.com/PerePala/VentiladorUPCManresa/blob/master/ESP32experiments/mqtt_sender.py)

# Monitoring the packets

```
sudo tcpdump -n port 1883

13:18:55.750694 IP 192.168.1.25.51103 > 192.168.1.15.1883: Flags [P.], seq 85:87, ack 1, win 5740, length 2
13:18:55.750749 IP 192.168.1.15.1883 > 192.168.1.25.51103: Flags [.], ack 87, win 64071, length 0
13:18:55.754016 IP 192.168.1.25.51103 > 192.168.1.15.1883: Flags [P.], seq 87:102, ack 1, win 5740, length 15
13:18:55.754056 IP 192.168.1.15.1883 > 192.168.1.25.51103: Flags [.], ack 102, win 64071, length 0
13:18:56.774691 IP 192.168.1.25.51103 > 192.168.1.15.1883: Flags [P.], seq 102:104, ack 1, win 5740, length 2
13:18:56.774740 IP 192.168.1.15.1883 > 192.168.1.25.51103: Flags [.], ack 104, win 64071, length 0
13:18:56.777750 IP 192.168.1.25.51103 > 192.168.1.15.1883: Flags [P.], seq 104:119, ack 1, win 5740, length 15
13:18:56.777773 IP 192.168.1.15.1883 > 192.168.1.25.51103: Flags [.], ack 119, win 64071, length 0
```

This seems to indicate that every MQTT transaction starts with a 2 byte packet, followed by the actual payload (15 bytes in this case). As we are transmitting ```pot > 132``` this means that there is some additional overhead.
