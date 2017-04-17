//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

// Settings - Debug
#define DEBUG true

// Settings - Bluetooth
#define DEBUG_BLUETOOTH true
#define BLUETOOTH_ENABLED false
#define BLUETOOTH_SETUP false
#define BLUETOOTH_ALIVE 3

// Settings - Gyro
#define DEBUG_GYRO false
#define DEBUG_ACCEL false
#define GYRO_ENABLED true
#define GYRO_CALIBRATE false

// Settings - Controller
#define DEBUG_CONTROLLER true
#define CTLR_ENABLED true
#define CTLR_SPEED_START 63
#define CTLR_SPEED_LOW_RANGE 0 //60
#define CTLR_SPEED_HIGH_RANGE 255 //70

// Settings - Controller Balance
#define CTLR_LAUNCH_FORCE 10
#define CTLR_BALANCE_FORCE 0.10
#define CTLR_BALANCE_TIMER 8000
#define CTLR_BALANCE_MULTIPLIER 50

// Settings - Controller PID
#define CTLR_PID_SPEED 10
#define CTLR_PID_P 1
#define CTLR_PID_I 0
#define CTLR_PID_D 1

// Motor Controllers
#include "MotorController.h";
extern MotorController motorFrontLeft(1, 3);
extern MotorController motorFrontRight(2, 11);
extern MotorController motorBackLeft(3, 5);
extern MotorController motorBackRight(4, 6);

// Helper Dependencies
#include "Helpers.h";

// Gyro Receiver
#include "GyroReceiver.h";
extern GyroReceiver gyro(isDebug(DEBUG_GYRO), isDebug(DEBUG_ACCEL));

// Bluetooth Receiver
#include "BluetoothReceiver.h";
extern BluetoothReceiver bluetooth(5, 6, isDebug(DEBUG_BLUETOOTH));

// Speed Controller
#include "SpeedController.h";
extern SpeedController speedController(isDebug(DEBUG_CONTROLLER));

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
        debug("-- Debug Started --");
        debug("");

        debug("+ Debugging initialized.");
    }

    // Setup Bluetooth module
    if(BLUETOOTH_ENABLED) {
        bluetooth.setup();
        debug("+ Bluetooth initialized.");
    }

    // Setup Gyro module
    if(GYRO_ENABLED) {
        // Setup Gyro
        delay(1000);
        gyro.setup();
        debug("+ Gyro initialized.");

        // Should calibrate
        if(GYRO_CALIBRATE) {
            // Send calibration warning
            debug("+ Gyro calibration starting in 2 seconds.");
            delay(2000);

            // Calibrate Gyro
            gyro.calibrate();
            debug("+ Gyro calibrated.");
        }
    }

    // Setup Speed Controller module
    if(CTLR_ENABLED) {
        speedController.setup();
        debug("+ Speed Controller initialized.");
    }

    // Completed
    debug("");
    debug("-- Setup Completed --");
    debug("");

    delay(4000);
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
    if(GYRO_ENABLED) {
        gyro.retrieve();
    }
    
    // Retrieve Bluetooth readings
    if(BLUETOOTH_ENABLED) {
        bluetooth.retrieve();
    }

    // Handle Speed Controller
    if(CTLR_ENABLED) {
        speedController.handle();
    }
}