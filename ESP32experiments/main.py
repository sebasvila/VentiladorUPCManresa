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

