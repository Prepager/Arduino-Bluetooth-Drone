//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

// Settings - Debug
#define DEBUG true
#define DEBUG_GYRO false
#define DEBUG_ACCEL false
#define DEBUG_BLUETOOTH true

// Settings - Bluetooth
#define BLUETOOTH_SERIAL false
#define BLUETOOTH_ALIVE 3

// Includes
#include "Helpers.h";
#include "GyroReceiver.h";
#include "BluetoothReceiver.h";

// Classes
GyroReceiver gyro;
BluetoothReceiver bluetooth(10, 11);

/*
 * Function: setup
 * ----------------------------
 * Setup function for Arduino (Run once).
 *
 * @returns: void
 */
void setup() {
    // Begin serial in debugging
    if(DEBUG) {
        Serial.begin(9600);
        debug("-- Debugging initialized --");
        debug("");
    }

    // Setup Gyro module
    gyro.setup();
    debug("+ Gyro initialized.");

    // Setup Bluetooth module
    bluetooth.setup(BLUETOOTH_ALIVE);
    debug("+ Bluetooth initialized.");

    // Completed
    debug("");
    debug("-- Setup Completed --");
    debug("");
}

/*
 * Function: loop
 * ----------------------------
 * Loop function for Arduino (Run every tick).
 *
 * @returns: void
 */
void loop() {
    // Retrieve gyro readings
    gyro.retrieve(DEBUG && DEBUG_GYRO, DEBUG && DEBUG_ACCEL);
    
    // Retrieve bluetooth readings
    bluetooth.retrieve(DEBUG && DEBUG_BLUETOOTH);
}