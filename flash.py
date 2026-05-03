import serial

def crc32_mpeg2(data_bytes, poly=0x04C11DB7, init=0xFFFFFFFF, log_file="crc_log.txt"):
    crc = init

    with open(log_file, "w") as f:
        for i, byte in enumerate(data_bytes):
            crc ^= (byte << 24)

            for _ in range(8):
                if crc & 0x80000000:
                    crc = ((crc << 1) ^ poly) & 0xFFFFFFFF
                else:
                    crc = (crc << 1) & 0xFFFFFFFF

            # log after processing each byte
            f.write(f"After byte {i} (0x{byte:02X}): CRC = 0x{crc:08X}\n")

    return crc


# Open the port
print("opening port")
ser = serial.Serial('COM7', 9600)

# send R
ser.write(bytes([0x52]))

# wait for slot
slot = ser.read(1)

# 0x61 is ASCII 'a'
if slot == b'\x61':
    with open("application/app_slot_a.bin", "rb") as f:
        payload = f.read()
    print("Branching to Slot A")
else:
    with open("application/app_slot_b.bin", "rb") as f:
        payload = f.read()
    print(f"Branching to Slot B")

# send payload size
length = len(payload) # e.g., 500
length_bytes = length.to_bytes(2, byteorder='little')
ser.write(length_bytes)

print("waiting for payload confirmation")
# wait for payload confirmation
ser.read(1)

# send R
ser.write(bytes([0x52]))
print("waiting for write confirmation")
ser.read(1)

#calc CRC for slot    
golden_crc = crc32_mpeg2(payload)

# Send the file in chunks
chunk_size = 1
for i in range(0, len(payload), chunk_size):
    chunk = payload[i:i + chunk_size]
    print(f"sending chunk")
    ser.write(chunk)
    # Wait for the bootloader to say "Ready for next chunk"
    print(f"waiting")
    response = ser.read(1)  

crc_bytes = golden_crc.to_bytes(4, byteorder='big') 
ser.write(crc_bytes)

    