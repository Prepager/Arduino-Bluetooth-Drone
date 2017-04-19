//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

// Dependencies
#include "Servo.h";

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

        int pos, pin;
        
        float speed = SPEED_HOVER,
        baseSpeed = SPEED_HOVER,
        nextSpeed = SPEED_HOVER;

        Servo * motor;

        MotorController(int position, int pinNum);
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
MotorController::MotorController(int position, int pinNum) {
    // Variables
    pos = position;
    pin = pinNum;
}

/*
 * Function: setup
 * ----------------------------
 * Setup motor controller class.
 *
 * @returns: void
 */
void MotorController::setup() {
    // Setup pin mode
    motor = new Servo();
    motor->attach(pin);
    motor->writeMicroseconds(SPEED_MINIMAL);
}

/*
 * Function: handle
 * ----------------------------
 * Handle the motor speed and rotation.
 *
 * @returns: void
 */
void MotorController::handle() {
    // Manage speed
    speed = constrain(nextSpeed, SPEED_RANGE_MIN, SPEED_RANGE_MAX);

    // Write speed
    motor->writeMicroseconds(speed);
}