import serial, time, sys, os

# Python uploader.py COM4 cute.bin 0x000000



def upload_file(serial_port, file_path, target_address_str):
    try:
        target_address = int(target_address_str, 16)
        file_size = os.path.getsize(file_path)
    except Exception as e:
        print(f"Error: Invalid argument. {e}")
        return

    print(f"\n--- Uploading {file_path} ---")
    print(f"Port: {serial_port}, Address: 0x{target_address:06X}, Size: {file_size} bytes")

    try:
        ser = serial.Serial(serial_port, 115200, timeout=5)
        time.sleep(2)
    except serial.SerialException as e:
        print(f"Error opening serial port {serial_port}: {e}")
        return

    response = ser.read_until(b'READY\n').decode().strip()
    if "READY" not in response:
        print(f"Error: ESP32 not ready. Last message: {response}")
        ser.close()
        return
        
    print("ESP32 is ready. Sending command...")
    ser.write(f"WRITE {target_address} {file_size}\n".encode())

    response = ser.read_until(b'\n').decode().strip()
    if not response.startswith("OK"):
        print(f"Error from ESP32: {response}")
        ser.close()
        return
        
    print("Starting file transfer...")
    with open(file_path, 'rb') as f:
        start_time = time.time()
        bytes_sent = 0
        while True:
            chunk = f.read(256)
            if not chunk: break
            ser.write(chunk)
            bytes_sent += len(chunk)
            print(f"Progress: {(bytes_sent/file_size)*100:.2f}%", end='\r')
        
    print("\nFile sent. Waiting for confirmation...")
    
    response = ser.read_until(b'\n').decode().strip()
    if response == "SUCCESS":
        print(f"✅ Success! Upload completed in {time.time() - start_time:.2f} seconds.")
    else:
        print(f"❌ Failure! ESP32 reported: {response}")
    ser.close()

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("Usage: python uploader.py <COM_PORT> <FILE_PATH> <HEX_ADDRESS>")
        sys.exit(1)
    upload_file(sys.argv[1], sys.argv[2], sys.argv[3])