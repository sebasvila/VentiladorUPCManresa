sudo apt install mosquitto mosquitto-clients

On one window, subscribe to a feed named *test*

mosquitto_sub -h localhost -t test

On the other window, publish something related to *test*

mosquitto_pub -h localhost -t test -m 'Hi'


A tutorial to [install mosquitto](https://www.digitalocean.com/community/tutorials/how-to-install-and-secure-the-mosquitto-mqtt-messaging-broker-on-ubuntu-18-04).

A nice [tutorial](https://randomnerdtutorials.com/micropython-mqtt-esp32-esp8266/) to use MQTT on ESP32

