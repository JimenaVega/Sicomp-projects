# -*- coding: utf-8 -*-

import os 
import matplotlib.pyplot as plt
import matplotlib.animation as anim
import numpy as np
import time

DEVICE_FILE = "/dev/gpio_rbp"

sensorName = ""
yName = ""

# def prompt():
#     print("SEÑALES A MOSTRAR:")
#     print("[0] Exit program")
#     print("[1] Sensor de temperatura")
#     print("[2] Fotosensor")


# def animationFrame(i):
#     print("#{0}".format(i))

#     ax1.clear()
#     ax1.bar([sensorName], y, align='center', alpha=0.5)
#     ax1.set_ylim([0, 100])
#     ax1.set_ylabel(yName)
#     ax1.set_xlabel("time")
    
#In[0]:

# fig = plt.figure()
# ax1 = fig.add_subplot(1,1,1)

# r = 0

# while(1):

#     r = 0
#     prompt()
#     signal = input("Ingrese el numero señal a graficar\n>>")

    # if(signal == "0"):
    #     break
    # elif (signal == "1"):
    #     print("Señal [{0}]".format(signal))

        # sensorName = "Señal 1"
        # yName = "luz"
signal = "1"
fd = open(DEVICE_FILE, "w")
fd.write(signal)
fd.close()



    # elif (signal == "2"):
    #     print("Señal 2")

    #     sensorName = "Señal 2"
    #     yName = "Temperatura"

    #     fd = open(DEVICE_FILE, "w")
    #     fd.write(signal)
    #     fd.close()

    # else:
    #     print("Error. Señal inexistente.")
    


    # for i in range(100):
        
time.sleep(1)

fd = open(DEVICE_FILE, "r")
r = ord(fd.read(1))
print("Lo leido es [{0}]".format(r))
fd.close()
# animation = anim.FuncAnimation(fig, func=animation_frame, frames=np.arange(0,10))
    #animation = anim.FuncAnimation(fig, func=animationFrame, frames=100, interval=20)
    #plt.show()

    #fd.close()


    

