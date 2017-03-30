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
#define DEBUG_CONTROLLER true

// Settings - Bluetooth
#define BLUETOOTH_ALIVE 3

// Motor Controllers
#include "MotorController.h";
extern MotorController motorFrontLeft(9);
extern MotorController motorFrontRight(10);
extern MotorController motorBackLeft(3);
extern MotorController motorBackRight(11);

// Other Dependencies
#include "Helpers.h";

// Receivers
#include "GyroReceiver.h";
GyroReceiver gyro(isDebug(DEBUG_GYRO), isDebug(DEBUG_ACCEL));

#include "BluetoothReceiver.h";
BluetoothReceiver bluetooth(5, 6, isDebug(DEBUG_BLUETOOTH), BLUETOOTH_ALIVE);

// Speed Controller
#include "SpeedController.h";
SpeedController speedController(isDebug(DEBUG_CONTROLLER));

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
    bluetooth.setup();
    debug("+ Bluetooth initialized.");

    // Setup Motor Controller module
    speedController.frontLeft = &motorFrontLeft;
    speedController.frontRight = &motorFrontRight;
    speedController.backLeft = &motorBackLeft;
    speedController.backRight = &motorBackRight;
    debug("+ Motor Controllers initialized.");

    // Setup Speed Controller module
    speedController.setup();
    debug("+ Speed Controller initialized.");

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
    // Retrieve Gyro readings
    //gyro.retrieve();
    
    // Retrieve Bluetooth readings
    //bluetooth.retrieve();

    // Handle Speed Controller
    speedController.handle(gyro, bluetooth);
}