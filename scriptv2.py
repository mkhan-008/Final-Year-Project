import serial
import csv
from datetime import datetime
import os

print("Start programm")
def appendHeader():
    with open('data.csv', mode='a') as file:
        writer = csv.writer(file, delimiter=',',
                            quotechar='"', quoting=csv.QUOTE_MINIMAL)
        writer.writerow(["Timestamp", "Access"])
arduino = serial.Serial('COM3', 9600, timeout=.1)
while True:
    # the last bit gets rid of the new-line chars

    data = arduino.readline()[:-2]
    if data:
        print("raw data",data)
        data = data.decode()
        now = datetime.now()
        dt_string = now.strftime("%d/%m/%Y %H:%M:%S")
        data = dt_string + "," + data
        if os.path.isfile('data.csv'):
            with open('data.csv', mode='r') as file:
                if("Timestamp" not in file.readline()):
                    appendHeader()
        else:
            appendHeader()
        with open('data.csv', mode='a') as file:
            writer = csv.writer(file, delimiter=',',
            quotechar='"', quoting=csv.QUOTE_MINIMAL)
            print(data.split(','))
            writer.writerow(data.split(','))
