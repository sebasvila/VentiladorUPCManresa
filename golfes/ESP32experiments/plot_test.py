import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
from time import time


# Parameters
x_len = 200         # Number of points to display
y_range = [-10, 50]  # Range of possible Y values to display

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

    # Produce a new value
    temp_c = 20+20*np.cos(2*np.pi*time()/3)

    # Add y to list
    ys.append(temp_c)

    # Limit y list to set number of items
    ys = ys[-x_len:]

    # Update line with new Y values
    line.set_ydata(ys)

    return line,

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig,
    animate,
    fargs=(ys,),
    interval=50,
    blit=True)
plt.show()

