import random
from itertools import count
import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

DEVICE_FILE = "/dev/gpio_rbp"

plt.style.use('fivethirtyeight')

x_vals = []
y_vals = []

index = count()

def prompt():
    print("SEÑALES A MOSTRAR:")
    print("[0] Exit")
    print("[1] Channel 1")
    print("[2] Channel 2")

globalLabel = "Channel 2"

def animate(i):
    data = pd.read_csv('data.csv')
    x = data['x_value']
    y1 = data['total_1']

    plt.cla()

    plt.plot(x, y1, 'r',label=globalLabel)

    plt.legend(loc='upper left')
    plt.tight_layout()

# In[0]: Main

while(1):
    prompt()
    signal = input("Ingrese el numero señal a graficar\n>>")

    if(signal == "1"):
        globalLabel = "Channel 1"
    elif (signal == "2"):
         globalLabel = "Channel 2"
    elif (signal == "0"):
        break
    else:
        print("Error in input.")
        break
    
    fd = open(DEVICE_FILE, "w")
    fd.write(signal)
    fd.close()

    ani = FuncAnimation(plt.gcf(), animate, interval=10)

    plt.tight_layout()
    plt.show()