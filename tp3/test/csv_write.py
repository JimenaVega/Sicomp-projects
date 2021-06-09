import csv
import random
import time
DEVICE_FILE = "/dev/gpio_rbp"


x_value = 0
total_1 = 0


fieldnames = ["x_value", "total_1"]


with open('data.csv', 'w') as csv_file:
    csv_writer = csv.DictWriter(csv_file, fieldnames=fieldnames)
    csv_writer.writeheader()

while True:

    with open('data.csv', 'a') as csv_file:
        csv_writer = csv.DictWriter(csv_file, fieldnames=fieldnames)

        info = {
            "x_value": x_value,
            "total_1": total_1
        }

        csv_writer.writerow(info)
        print(x_value, total_1)


        fd = open(DEVICE_FILE, "r")
        r = ord(fd.read(1))
        #print("Lo leido es [{0}]".format(r))
        fd.close()

        x_value += 1
        total_1 = r

    time.sleep(1)