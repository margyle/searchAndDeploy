import mysql.connector as mysql
import serial

#set up serial (ls /dev/tty* to find your serial address)
ser = serial.Serial('/dev/tty.usbmodem14311101',19200)
ser.write(str.encode('<0,0,0>'))

db = mysql.connect(
    host = "localhost",
    user = "****",
    passwd = "****",
    database = "****"
)

inputPart = input("Enter Part To Locate: ")
cursor = db.cursor()

## query
cursor.execute("SELECT * FROM items WHERE partName = %s",(inputPart,))

## get records
records = cursor.fetchall()

## Showing the data
for record in records:
    print(record[1] + " Location is: "+ record[2]+ ". Coordinates are x = " + str(record[3]) + ",y = " + str(record[4]))
    ser.write(str.encode('<0,' + str(record[3]) + ',' + str(record[4])+ '>'))
