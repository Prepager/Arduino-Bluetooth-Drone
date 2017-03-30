//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

// Prevent double inclusion
#pragma once

// Dependencies
#include "TimerOne.h";

/*
 * Class: SpeedController
 * ----------------------------
 * Control drone motors.
 *
 * @returns: class
 */
class SpeedController {

    // Public
    public:

        bool debugSpeed;

        MotorController * frontLeft, * frontRight, * backLeft, * backRight;

        SpeedController(bool debug = false);
        void setup();
        
        void handle(GyroReceiver gyroReceiver, BluetoothReceiver bluetoothReceiver);
        static void callback();

};

/*
 * Function: SpeedController
 * ----------------------------
 * Initialize the SpeedController class.
 *
 * @returns: void
 */
SpeedController::SpeedController(bool debug = false) {
    // Variables
    debugSpeed = debug;
}

/*
 * Function: setup
 * ----------------------------
 * Setup motor controller class.
 *
 * @returns: void
 */
void SpeedController::setup() {
    // Setup motors
    frontLeft->setup();
    frontRight->setup();
    backLeft->setup();
    backRight->setup();

    // Start timer one
    Timer1.initialize(10);
    Timer1.attachInterrupt(SpeedController::callback);
    debug("+ TimerOne initialized.");
}

/*
 * Function: handle
 * ----------------------------
 * Handle the speed of the motors.
 *
 * @returns: void
 */
void SpeedController::handle(GyroReceiver gyroReceiver, BluetoothReceiver bluetoothReceiver) {
    //
}

/*
 * Function: callback
 * ----------------------------
 * Handle the movement of the motors.
 *
 * @returns: void
 */
static void SpeedController::callback() {
    // Handle motor movement
    motorFrontLeft.handle();
    motorFrontRight.handle();
    motorBackLeft.handle();
    motorBackRight.handle();
}