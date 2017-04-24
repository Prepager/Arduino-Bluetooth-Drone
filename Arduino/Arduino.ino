//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

// Core Dependencies
#include "Settings.h";
#include "Helpers.h";

// Global Variables
extern long loopStartTime = millis();
extern float executionTime = 0.01;

// Gyro Receiver
#include "GyroReceiver.h";
extern GyroReceiver gyro(GYRO_CALIBRATE);

// Bluetooth Receiver
#include "BluetoothReceiver.h";
extern BluetoothReceiver bluetooth(PIN_BLUETOOTH_TX, PIN_BLUETOOTH_RX);

// Motor Controllers
#include "MotorController.h";
extern MotorController motorFrontLeft(0, PIN_FRONTLEFT);
extern MotorController motorFrontRight(1, PIN_FRONTRIGHT);
extern MotorController motorBackRight(2, PIN_BACKRIGHT);
extern MotorController motorBackLeft(3, PIN_BACKLEFT);

extern MotorController motorList[4] = {
    motorFrontLeft, motorFrontRight,
    motorBackRight, motorBackLeft
};

// Speed Controller
#include "SpeedController.h";
extern SpeedController speedController(false);

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
    if(MODULE_BLUETOOTH) {
        bluetooth.setup();
        debug("+ Bluetooth initialized.");
    }

    // Setup Gyro module
    if(MODULE_GYROSCOPE) {
        // Setup Gyro
        gyro.setup();
        debug("+ Gyro initialized.");
    }

    // Setup Speed Controller module
    if(MODULE_CONTROLLER) {
        speedController.setup();
        debug("+ Speed Controller initialized.");
    }

    // Completed
    debug("");
    debug("-- Setup Completed --");
    debug("");

    delayCountdown(5000);
    delay("");
    debug("+ Ready for launch.");
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
    // Save current time to calculate execution time
    loopStartTime = millis();

    // Retrieve Gyro readings
    if(MODULE_GYROSCOPE) {
        gyro.retrieve();
    }
    
    // Retrieve Bluetooth readings
    if(MODULE_BLUETOOTH) {
        bluetooth.retrieve();
    }

    // Handle Speed Controller
    if(MODULE_CONTROLLER) {
        speedController.handle();
    }

    // Save execution time and convert millis to seconds
    executionTime = (millis() - loopStartTime) * 0.001;

    // Output speed
    if(isDebug(DEBUG_EXECUTION)) {
        Serial.println(executionTime);
    }
}