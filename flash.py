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
            f.write(f"After byte {i} (0x{byte:02X}): CRC = 0x{crc:08X}\n")
    return crc

def run_bootloader():
    # Constants
    CHUNK_SIZE = 500
    ACK = b'\x01'
    NACK = b'\x02'
    
    # Open the port
    print("opening COM7 port")
    try:
        ser = serial.Serial('COM7', 9600, timeout=5) # Added timeout for safety
    except Exception as e:
        print(f"Failed to open port: {e}")
        return

    # send R
    ser.write(bytes([0x52]))

    # wait for slot
    slot = ser.read(1)

    # 0x61 is ASCII 'a'
    if slot == b'\x61':
        with open("application/app_slot_a.bin", "rb") as f:
            payload = f.read()
        print("Choosing Slot A")
    else:
        with open("application/app_slot_b.bin", "rb") as f:
            payload = f.read()
        print(f"Choosing Slot B")

    # send payload size
    length = len(payload)
    length_bytes = length.to_bytes(2, byteorder='little')
    ser.write(length_bytes)
    print("sent payload length")

    # wait for payload confirmation
    response = ser.read(1)
    if response == ACK:
        print("Payload size confirmed. Starting transfer...")
    elif response == NACK:
        print("Error: Payload exceeds available flash memory.")
        return # Exit because payload is too large [cite: 180]
    else:
        print("Error: Unknown response to length check.")
        return

    # send R and wait for sync
    ser.write(bytes([0x52]))
    ser.read(1)

    # calc CRC for entire payload
    golden_crc = crc32_mpeg2(payload)

    # Send the file in chunks
    for i in range(0, len(payload), CHUNK_SIZE):
        chunk = payload[i:i + CHUNK_SIZE]
        
        # Note: You mentioned previously you didn't want padding, 
        # but your current code uses it. Keeping it as per your snippet.
        if len(chunk) < CHUNK_SIZE:
            chunk = chunk.ljust(CHUNK_SIZE, b'\xFF')

        # Atomic Packet: Calculate CRC for this specific chunk [cite: 85, 145]
        block_crc = crc32_mpeg2(chunk)
        crc_bytes = block_crc.to_bytes(4, byteorder='big')

        attempts = 0
        success = False
        while attempts < 3: 
            print(f"Sending block {i//CHUNK_SIZE} (Attempt {attempts + 1})...")
            ser.write(chunk)
            ser.write(crc_bytes) # Send CRC immediately after data [cite: 97, 131]

            response = ser.read(1) # Wait for confirmation [cite: 10, 87]

            if response == ACK:
                print(f"Block {i//CHUNK_SIZE} verified and flashed.")
                success = True
                break 
            elif response == NACK:
                print(f"Block {i//CHUNK_SIZE} CRC error. Retrying...")
                attempts += 1
            else:
                print("Timeout or unknown response. Retrying...")
                attempts += 1
        
        if not success:
            print("Failed to send block after 3 attempts. Aborting.")
            return # Exit on failure [cite: 181]

    # Finalize transfer
    final_crc_bytes = golden_crc.to_bytes(4, byteorder='big') 
    ser.write(final_crc_bytes)
    print("Sent CRC of entire payload")

if __name__ == "__main__":
    run_bootloader()