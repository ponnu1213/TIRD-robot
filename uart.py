import serial
ser = serial.Serial ("/dev/ttyS0")    #Open named port 
ser.baudrate = 9600
#Set baud rate to 9600
while 1:
   #ser.write(1)
   data = ser.read(10)                     #Read ten characters from serial port to data
  # ser.write(data)
   abc =chr(data[9])+chr(data[8])+chr(data[7])
   print(abc)                 #Send back the received data
ser.close()

