import RPi.GPIO as GPIO # Import Raspberry Pi GPIO library
import time

GPIO.setwarnings(False) # Ignore warning for now
GPIO.setmode(GPIO.BOARD) # Use physical pin numbering
GPIO.setup(8, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) # Set pin 10 to be an input pin and set initial value to be pulled low (off)


count = 0

while True: # Run forever
    time.sleep(0.1)
    if (GPIO.input(8) == GPIO.HIGH):
        print("Button was pushed! count = {0}".format(count))
        count = count + 1
