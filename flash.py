import serial

def crc32_mpeg2(data_bytes, poly=0x04C11DB7, init=0xFFFFFFFF):
    crc = init
    for byte in data_bytes:
        crc ^= (byte << 24)
        for _ in range(8):
            if crc & 0x80000000:
                crc = ((crc << 1) ^ poly) & 0xFFFFFFFF
            else:
                crc = (crc << 1) & 0xFFFFFFFF            
    return crc


# Open the port
print("opening port")
ser = serial.Serial('COM7', 9600)

with open("application/app.bin", "rb") as f:
    payload = f.read()
    
golden_crc = crc32_mpeg2(payload)

# Send the file in chunks
chunk_size = 1
for i in range(0, len(payload), chunk_size):
    chunk = payload[i:i + chunk_size]
    ser.write(chunk)
    # Wait for the bootloader to say "Ready for next chunk"
    response = ser.read(1)

crc_bytes = golden_crc.to_bytes(4, byteorder='big') 
ser.write(crc_bytes)

    