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
        
        float speed = CTLR_SPEED_START,
        baseSpeed = CTLR_SPEED_START,
        nextSpeed = CTLR_SPEED_START;

        Servo * motor;

        MotorController(int position, int pinNum);
        void setup();

        //float handleSpeed();
        //float speedChange();
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
    motor->writeMicroseconds(CTLR_SPEED_BASE);
}

/*
 * Function: speedChange
 * ----------------------------
 * Handle the motor speed change.
 *
 * @returns: float
 */
/*float MotorController::speedChange() {
    // Return
    return 0.0003 * speed;
}*/

/*
 * Function: handleSpeed
 * ----------------------------
 * Handle the motor speed.
 *
 * @returns: float
 */
/*float MotorController::handleSpeed() {
    // Variables
    float tempSpeed;

    // Change speed
    tempSpeed = 0;
    /*if(speed > nextSpeed) {
        tempSpeed = speed - speedChange();
    } else if(speed < nextSpeed) {
        tempSpeed = speed + speedChange();
    } else {
        return speed;
    }*/

    // Ensure speed limits
    /*tempSpeed = constrain(tempSpeed, CTLR_SPEED_LOW_RANGE, CTLR_SPEED_HIGH_RANGE);

    // Return
    return tempSpeed;
}*/

/*
 * Function: handle
 * ----------------------------
 * Handle the motor speed and rotation.
 *
 * @returns: void
 */
void MotorController::handle() {
    // Manage speed
    speed = constrain(nextSpeed, CTLR_SPEED_LOW_RANGE, CTLR_SPEED_HIGH_RANGE);

    // Write speed
    //if(pos == 1 || pos == 3) {
        motor->writeMicroseconds(speed);
    /*} else {
        motor->writeMicroseconds(CTLR_SPEED_BASE);
    }*/

    // Debug output
    /*Serial.print(pos);
    Serial.print(" - ");
    Serial.print(speed);
    Serial.print("   ");

    if(pos == 3) {
        Serial.println("");
    }*/
}