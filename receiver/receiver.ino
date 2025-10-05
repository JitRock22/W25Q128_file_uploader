// #include <SPI.h>
// #include <SPIMemory.h>

// // --- CONFIGURATION ---
// // Define the Chip Select pin for the W25Q128 connected to your ESP32
// const int CS_PIN = 5;
// SPIFlash flash(CS_PIN);
// const int SECTOR_SIZE = 4096;

// void setup() {
//   Serial.begin(115200);
//   while (!Serial);

//   if (!flash.begin()) {
//     Serial.println("ERROR: Flash init failed");
//     while(1); // Stop if the chip isn't found
//   }
//   // This "READY" signal is crucial. It tells the Python script
//   // that the ESP32 is booted up and ready to receive a command.
//   Serial.println("READY");
// }

// void loop() {
//   // Check if the PC has sent a command
//   if (Serial.available() > 0) {
//     String command = Serial.readStringUntil(' ');
//     command.trim();

//     // We only have one command: "WRITE"
//     if (command == "WRITE") {
//       handleWriteCommand();
//     }
//   }
// }

// void handleWriteCommand() {
//   // 1. Read the address and size sent by the Python script
//   uint32_t address = Serial.parseInt();
//   uint32_t size = Serial.parseInt();
//   while(Serial.read() != '\n'); // Clear the rest of the command line

//   // 2. Erase the necessary space on the flash chip
//   uint32_t sectorsToErase = (size / SECTOR_SIZE) + 1;
//   for (uint32_t i = 0; i < sectorsToErase; ++i) {
//     if (!flash.eraseSector(address + i * SECTOR_SIZE)) {
//       Serial.printf("ERROR: Failed to erase sector at 0x%06X\n", address + i * SECTOR_SIZE);
//       return;
//     }
//   }

//   // 3. Send "OK" back to the PC to signal that we are ready for the binary data
//   Serial.printf("OK %d\n", size); 
//   Serial.flush();

//   // 4. Read the raw binary data from the PC and write it to the flash chip
//   byte buffer[256];
//   uint32_t bytesWritten = 0;
//   while (bytesWritten < size) {
//     // int bytesToRead = min(sizeof(buffer), size - bytesWritten);
//     int bytesToRead = min((uint32_t)sizeof(buffer), size - bytesWritten);
//     int readCount = Serial.readBytes(buffer, bytesToRead);

//     if (!flash.writeByteArray(address + bytesWritten, buffer, readCount)) {
//         Serial.println("ERROR: Flash write failed");
//         return;
//     }
//     bytesWritten += readCount;
//   }

//   // 5. Send a final "SUCCESS" message to the PC
//   Serial.println("SUCCESS");
// }





#include <SPI.h>
#include <SPIMemory.h>

// --- CONFIGURATION ---
// Define the Chip Select pin for the W25Q128 connected to your ESP32
const int CS_PIN = 5;
SPIFlash flash(CS_PIN);
const int SECTOR_SIZE = 4096;

// --- PROTOTYPES FOR NEW FUNCTIONS ---
void handleEraseChipCommand(); // ADD THIS LINE

void setup() {
  Serial.begin(115200);
  while (!Serial);

  if (!flash.begin()) {
    Serial.println("ERROR: Flash init failed");
    while(1); // Stop if the chip isn't found
  }
  // This "READY" signal is crucial. It tells the Python script
  // that the ESP32 is booted up and ready to receive a command.
  Serial.println("READY");
}

void loop() {
  // Check if the PC has sent a command
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil(' ');
    command.trim();

    // We now have two commands: "WRITE" and "ERASE_CHIP"
    if (command == "WRITE") {
      handleWriteCommand();
    } else if (command == "ERASE_CHIP") { // ADD THIS 'ELSE IF' BLOCK
      handleEraseChipCommand();
    }
  }
}

// --- ADD THIS ENTIRE NEW FUNCTION ---
// This function handles the command to erase the entire chip.
void handleEraseChipCommand() {
  // Let the Python script know we've started the long erase process
  Serial.println("INFO: Starting chip erase. This will take a while...");
  Serial.flush(); // Ensure the message is sent before the long operation
  
  if (flash.eraseChip()) {
    // Send a success message on completion
    Serial.println("SUCCESS");
  } else {
    // Send an error message if it fails
    Serial.println("ERROR: Chip erase failed");
  }
}

void handleWriteCommand() {
  // 1. Read the address and size sent by the Python script
  uint32_t address = Serial.parseInt();
  uint32_t size = Serial.parseInt();
  while(Serial.read() != '\n'); // Clear the rest of the command line

  // 2. Erase the necessary space on the flash chip
  uint32_t sectorsToErase = (size / SECTOR_SIZE) + 1;
  for (uint32_t i = 0; i < sectorsToErase; ++i) {
    if (!flash.eraseSector(address + i * SECTOR_SIZE)) {
      Serial.printf("ERROR: Failed to erase sector at 0x%06X\n", address + i * SECTOR_SIZE);
      return;
    }
  }

  // 3. Send "OK" back to the PC to signal that we are ready for the binary data
  Serial.printf("OK %d\n", size); 
  Serial.flush();

  // 4. Read the raw binary data from the PC and write it to the flash chip
  byte buffer[256];
  uint32_t bytesWritten = 0;
  while (bytesWritten < size) {
    int bytesToRead = min((uint32_t)sizeof(buffer), size - bytesWritten);
    int readCount = Serial.readBytes(buffer, bytesToRead);

    if (!flash.writeByteArray(address + bytesWritten, buffer, readCount)) {
        Serial.println("ERROR: Flash write failed");
        return;
    }
    bytesWritten += readCount;
  }

  // 5. Send a final "SUCCESS" message to the PC
  Serial.println("SUCCESS");
}