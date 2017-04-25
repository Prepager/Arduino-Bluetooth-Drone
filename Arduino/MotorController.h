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

        bool debugSpeed;
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

    // Debugging
    debugSpeed = isDebug(DEBUG_CONTROLLER);
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
    int speedSwitch = map(analogRead(0), 0, 1023, 0, 250);
    speed = constrain(nextSpeed+speedSwitch, SPEED_RANGE_MIN, SPEED_RANGE_MAX);

    // Multiply speed offsets
    if(speed != SPEED_MINIMAL) {
        switch(pos) {
            case 0: speed *= MOTOR_FL_OFFSET; break;
            case 1: speed *= MOTOR_FR_OFFSET; break;
            case 2: speed *= MOTOR_BR_OFFSET; break;
            case 3: speed *= MOTOR_BL_OFFSET; break;
        }
    }

    // Output current speed
    if(debugSpeed) {
        Serial.print(pos);
        Serial.print(" # ");
        Serial.print(speed);
        Serial.print("   ");

        if(pos == 3) {
            Serial.println("");
        }
    }

    // Write speed to motor
    motor->writeMicroseconds(speed);
}