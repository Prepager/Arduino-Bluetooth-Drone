//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

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
    pinMode(pin, OUTPUT);

    // Setup launch speed
    nextSpeed = CTLR_SPEED_RANGE;
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
    return 0.005 * speed;
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
    nextSpeed = constrain(speed + (CTLR_BALANCE_MULTIPLIER * value), 0, CTLR_SPEED_RANGE);
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
    tempSpeed = constrain(tempSpeed, 0, CTLR_SPEED_RANGE);

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
    analogWrite(pin, speed);
}