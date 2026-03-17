import serial

# Open the port
print("opening port")
ser = serial.Serial('COM7', 9600)

with open("application/app.bin", "rb") as f:
    payload = f.read()

# Send the file in chunks
chunk_size = 1
for i in range(0, len(payload), chunk_size):
    chunk = payload[i:i + chunk_size]
    ser.write(chunk)
    # Wait for the bootloader to say "Ready for next chunk"
    response = ser.read(1)
    print(response);