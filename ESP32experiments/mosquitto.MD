sudo apt install mosquitto mosquitto-clients

On one window, subscribe to a feed named *test*
mosquitto_sub -h localhost -t test

On the other window, publish something related to *test*
mosquitto_pub -h localhost -t test -m 'Hi'
