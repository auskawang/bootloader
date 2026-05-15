import serial

# Open the port
print("opening COM7 port")
try:
	ser = serial.Serial('COM7', 9600, timeout=5) # Added timeout for safety
except Exception as e:
	print(f"Failed to open port: {e}")

# send R
ser.write(bytes([0xA5]))
ser.write(bytes([0x5A]))
ser.write(bytes([0x7E]))


print("Sent byte")
