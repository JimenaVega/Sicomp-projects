# -*- coding: utf-8 -*-
#import matplotlib.pyplot as plt
import os 

DEVICE_FILE = "/dev/gpio_rbp"

xs = [0, 1, 2, 3, 4, 5, 6, 7]
ys = [1, 0.3, -2.3, 5.1, 7.6, -0.2, -1.8, 4]

#plt.plot(xs, ys)
#plt.show()

def prompt():
    print("SEÑALES A MOSTRAR:")
    print("[0] Exit program")
    print("[1] Sensor de temperatora")
    print("[2] Fotosensor")

#In[0]:


while(1):

    prompt()
    signal = input("Ingrese el numero señal a graficar\n>>")

    if(int(signal) == 0):
        break

    test = 'sudo echo "{0}" > {1}'.format(signal, DEVICE_FILE)
    print (test)
    os.system(test)


    

