# Complete project details at https://RandomNerdTutorials.com

def connect():
  global client_id, mqtt_server
  client = MQTTClient(client_id, mqtt_server, user=mqtt_userID, password=mqtt_password)
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

client_id = ubinascii.hexlify(machine.unique_id())
topic_sub = b'notification'
topic_pub = b'test'

message_interval = 500

try:
  client = connect()

except OSError as e:
  restart_and_reconnect()

last_message = time.ticks_ms()
while True:
  try:
    client.check_msg()
    if (time.ticks_ms() - last_message) > message_interval:
      msg = b'pot > %d' % pot.read()
      client.publish(topic_pub, msg)
      last_message = time.time()
      
  except OSError as e:
    restart_and_reconnect()

