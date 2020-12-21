# plotting tools
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
from time import time

# mqtt object
import paho.mqtt.client as mqtt

# System credentials
from myconfig import *

new_val=False
val=0


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


####################################### Plotting stuff ##############
# Parameters
x_len = 200         # Number of points to display
y_range = [0, 100]  # Range of possible Y values to display

# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = list(range(0, 200))
ys = [0] * x_len
ax.set_ylim(y_range)

# Create a blank line. We will update the line in animate
line, = ax.plot(xs, ys)

# Add labels
plt.title('Pressure over time')
plt.xlabel('Samples')
plt.ylabel('P')

# This function is called periodically from FuncAnimation
def animate(i, ys):
    global val
    global new_val
    
#    if new_val :
#        # Add y to list
#        ys.append(val)
#        new_val=False

#        # Limit y list to set number of items
#        ys = ys[-x_len:]

#    # Update line with new Y values
#    line.set_ydata(ys)

#    return line,
    # Produce a new value
    # Add y to list
    ys.append(val)

    # Limit y list to set number of items
    ys = ys[-x_len:]

    # Update line with new Y values
    line.set_ydata(ys)

    return line,

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig,
    animate,
    fargs=(ys,),
    interval=100,
    blit=True)








mqttc= mqtt.Client()
mqttc.on_connect=on_connect
mqttc.on_message=on_message

mqttc.username_pw_set(mqtt_userID,mqtt_password)

# BEWARE, outdated; see the follow up posts to use eu.thethings.network instead
mqttc.connect(mqtt_server)

# and listen to server

mqttc.loop_start()
plt.show()
#run = True
#while run:
#    mqttc.loop()


