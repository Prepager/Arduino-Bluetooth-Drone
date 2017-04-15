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
        float speed = 1, baseSpeed = 1, nextSpeed = 1;

        Servo * motor;

        MotorController(int position, int pinNum);
        void setup();

        void balanceSpeed(float value, bool inverse = false);
        float handleSpeed();
        float speedChange();
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
    //pinMode(pin, OUTPUT);
    motor = new Servo();
    motor->attach(pin);
    motor->write(20);

    // Setup launch speed
    nextSpeed = CTLR_SPEED_HIGH_RANGE;
}

/*
 * Function: speedChange
 * ----------------------------
 * Handle the motor speed change.
 *
 * @returns: float
 */
float MotorController::speedChange() {
    // Return
    return 0.0003 * speed;
}

/*
 * Function: balanceSpeed
 * ----------------------------
 * Handle the motor speed change on balance.
 *
 * @returns: void
 */
void MotorController::balanceSpeed(float value, bool inverse = false) {
    // Always make float positive
    value = abs(value);

    // Inverse value if needed
    if(inverse) {
        value = -value;
    }

    // Set next speed
    nextSpeed = constrain(speed + (CTLR_BALANCE_MULTIPLIER * value), CTLR_SPEED_LOW_RANGE, CTLR_SPEED_HIGH_RANGE);
}

/*
 * Function: handleSpeed
 * ----------------------------
 * Handle the motor speed.
 *
 * @returns: float
 */
float MotorController::handleSpeed() {
    // Variables
    float tempSpeed;

    // Change speed
    if(speed > nextSpeed) {
        tempSpeed = speed - speedChange();
    } else if(speed < nextSpeed) {
        tempSpeed = speed + speedChange();
    } else {
        return speed;
    }

    // Ensure speed limits
    tempSpeed = constrain(tempSpeed, CTLR_SPEED_LOW_RANGE, CTLR_SPEED_HIGH_RANGE);

    // Return
    return tempSpeed;
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
    speed = handleSpeed();

    // Write speed
    motor->write(speed);

    Serial.print(pos);
    Serial.print(" - ");
    Serial.println(speed);
    //analogWrite(pin, speed);
}