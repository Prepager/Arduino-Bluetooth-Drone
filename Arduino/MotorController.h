//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

// Prevent double inclusion
#pragma once

/*
 * Class: MotorController
 * ----------------------------
 * Control single drone motor.
 *
 * @returns: class
 */
class MotorController {

    // Public
    public:

        int speed;
        int pin1;

        MotorController(int pinOne);
        void setup();
        void handle();

};

/*
 * Function: MotorController
 * ----------------------------
 * Initialize the MotorController class.
 *
 * @returns: void
 */
MotorController::MotorController(int pinOne) {
    // Variables
    pin1 = pinOne;
}

/*
 * Function: setup
 * ----------------------------
 * Setup motor controller class.
 *
 * @returns: void
 */
void MotorController::setup() {
    // Pins
    pinMode(pin1, OUTPUT);
}

/*
 * Function: handle
 * ----------------------------
 * Handle motor controller class.
 *
 * @returns: void
 */
void MotorController::handle() {
    //
}