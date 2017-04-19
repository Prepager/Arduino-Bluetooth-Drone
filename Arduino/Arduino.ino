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
#define DEBUG_AXES true
#define GYRO_ENABLED true
#define GYRO_CALIBRATE false

// Settings - Controller
#define DEBUG_CONTROLLER true
#define CTLR_ENABLED true

// Settings - Controller Speed
#define CTLR_SPEED_BASE 750
#define CTLR_SPEED_START 1300
#define CTLR_SPEED_LOW_RANGE 1200
#define CTLR_SPEED_HIGH_RANGE 1500

// Settings - Controller Balance
#define CTLR_BALANCE_TIMER 8000
#define CTLR_BALANCE_MULTIPLIER 1

// Settings - Controller PID
#define CTLR_PID_P 1 //12 //3 // 2 //12
#define CTLR_PID_I 0 //0 // 0.03 // 0
#define CTLR_PID_D 0.05 //2 //0.8 //0.4 // 24 //2

// Variables
#define PI 3.1415926535

extern long loopStartTime = millis();
extern float executionTime = 0.01;

// Motor Controllers
#include "MotorController.h";
extern MotorController motorFrontLeft(0, 9);    // Wire: Brown
extern MotorController motorFrontRight(1, 11);  // Wire: Red
extern MotorController motorBackRight(2, 10);   // Wire: Yellow
extern MotorController motorBackLeft(3, 6);     // Wire: White

// Helper Dependencies
#include "Helpers.h";

// Gyro Receiver
#include "GyroReceiver.h";
extern GyroReceiver gyro(isDebug(DEBUG_GYRO), isDebug(DEBUG_ACCEL), isDebug(DEBUG_AXES));

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
int dead = 0;
void loop() {
    // Save current time to calculate execution time
    loopStartTime = millis();

    // Temp kill switch
    if(Serial.available()) {
        dead = 1;
    }

    if(dead) {
        for(int i = 0; i < 4; i++) {
            motorList[i].motor->writeMicroseconds(CTLR_SPEED_BASE);
        }

        return;
    }

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

    // Save execution time and convert millis to seconds
    executionTime = (millis() - loopStartTime) * 0.001;
}