# -*- coding: utf-8 -*-

import os 
import matplotlib.pyplot as plt
import matplotlib.animation as anim
import numpy as np
import time

DEVICE_FILE = "/dev/gpio_rbp"

sensorName = ""
yName = ""

def prompt():
    print("SEÑALES A MOSTRAR:")
    print("[0] Exit program")
    print("[1] Sensor de temperatura")
    print("[2] Fotosensor")


def animationFrame(i):
    print("#{0}".format(i))

    time.sleep(0.2)

    fd = open(DEVICE_FILE, "r")
    r = ord(fd.read(1))
    print("Lo leido es [{0}]".format(r))
    fd.close()

    ax1.clear()
    ax1.bar([sensorName], r, align='center', alpha=0.5)
    ax1.set_ylim([0, 100])
    ax1.set_ylabel("yName")
    ax1.set_xlabel("time")
    
#In[0]:

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)

# r = 0

while(1):

    prompt()
    signal = input("Ingrese el numero señal a graficar\n>>")
    fd = open(DEVICE_FILE, "w")

    # if(signal == "0"):
    #     break
    # elif (signal == "1"):
    #     print("Señal [{0}]".format(signal))

        # sensorName = "Señal 1"
        # yName = "luz"
    fd.write(signal)
    fd.close()

        


#animation = anim.FuncAnimation(fig, func=animation_frame, frames=np.arange(0,10))
    animation = anim.FuncAnimation(fig, func=animationFrame, frames=100, interval=20)
    plt.show()

    #fd.close()


    

