//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

// Dependencies
//#include "TimerOne.h";

// List
extern MotorController motorList[4] = {
    motorFrontLeft, motorFrontRight,
    motorBackRight, motorBackLeft
};

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

        bool debugSpeed, launched;
        long balancingDone = millis() + CTLR_BALANCE_TIMER;

        //long nextPid = millis();

        float pastX = 0, pastY = 0, pastZ = 0; // Integral
        float futureX, futureY, futureZ; // Derivative
        float errorX, errorY, errorZ; // Proportional

        float setpointX = -10, setpointY = 4, setpointZ = 0;
        float pastErrorX = 0, pastErrorY = 0, pastErrorZ = 0;

        SpeedController(bool debug = false);
        void setup();
        
        void handle();
        void handleBalance();
        //static void callback();

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
    for(int i = 0; i < 4; i++) {
        motorList[i].setup();
    }

    // Start timer one
    /*Timer1.initialize(TIMER_DELAY);
    Timer1.attachInterrupt(SpeedController::callback);
    debug("+ TimerOne initialized.");*/
}

/*
 * Function: handle
 * ----------------------------
 * Handle the speed of the motors.
 *
 * @returns: void
 */
void SpeedController::handle() {
    // Check launch balancing compleated
    /*if(!launched && millis() >= balancingDone) {
        // Save base values
        for(int i = 0; i < 4; i++) {
            int curSpeed = motorList[i].speed;
            motorList[i].baseSpeed = curSpeed;
            motorList[i].nextSpeed = curSpeed;
        }

        // Finish launch state
        launched = true;

        // Output debug message
        if(debugSpeed) {
            Serial.print("");
            Serial.println("+ Motors balanced. Ready for use.");
        }
    }*/

    // Check pid next run
    //if(millis() >= nextPid) {
        // Balance motors
        handleBalance();

        // Set next time
        //nextPid = millis() + CTLR_PID_SPEED;
    //}

    // Handle motor speed
    for(int i = 0; i < 4; i++) {
        motorList[i].handle();
    }
}

/*
 * Function: handleBalance
 * ----------------------------
 * Handle the balance of the motors.
 *
 * @returns: void
 */
void SpeedController::handleBalance() {
    // Read gyro radings
    float x = gyro.roll; //gyro.gForceX;
    float y = gyro.pitch; //gyro.gForceY;
    //float z = gyro.gForceZ;

    /*
        float pastX = 0, pastY = 0, pastZ = 0; // Integral
        float futureX, futureY, futureZ; // Derivative
        float errorX, errorY, errorZ; // Proportional

        float setpointX = 0, setpointY = 0, setpointZ = 0;
        float pastErrorX = 0, pastErrorY = 0, pastErrorZ = 0;
    */

    // X Axis
    errorX = setpointX - x;
    pastX += errorX * executionTime; //* CTLR_PID_SPEED;
    futureX = (errorX - pastErrorX) / executionTime; /// CTLR_PID_SPEED;
    int xOutput = (CTLR_PID_P * errorX) + (CTLR_PID_I * pastX) + (CTLR_PID_D * futureX);
    pastErrorX = errorX;

    // Y Axis
    errorY = setpointY - y;
    pastY += errorY * executionTime; //* CTLR_PID_SPEED;
    futureY = (errorY - pastErrorY) / executionTime; /// CTLR_PID_SPEED;
    int yOutput = (CTLR_PID_P * errorY) + (CTLR_PID_I * pastY) + (CTLR_PID_D * futureY);
    pastErrorY = errorY;

    // Z Axis
    /*errorZ = setpointZ - z;
    pastZ += errorZ * executionTime; //* CTLR_PID_SPEED;
    futureZ = (errorZ - pastErrorZ) / executionTime; /// CTLR_PID_SPEED;
    int zOutput = (CTLR_PID_P * errorZ) + (CTLR_PID_I * pastZ) + (CTLR_PID_D * futureZ);
    pastErrorZ = errorZ;*/

    // Write speed to motor
    motorList[0].nextSpeed = motorList[0].baseSpeed + (CTLR_BALANCE_MULTIPLIER * -xOutput);
    motorList[1].nextSpeed = motorList[1].baseSpeed + (CTLR_BALANCE_MULTIPLIER * yOutput);
    motorList[2].nextSpeed = motorList[2].baseSpeed + (CTLR_BALANCE_MULTIPLIER * xOutput);
    motorList[3].nextSpeed = motorList[3].baseSpeed + (CTLR_BALANCE_MULTIPLIER * -yOutput);
}

/*
 * Function: callback
 * ----------------------------
 * Handle the movement of the motors.
 *
 * @returns: void
 */
/*extern int callbackTimer = 0;
static void SpeedController::callback() {
    // Loop through motors
    for(int i = 0; i < 4; i++) {
        // Handle motor speed
        motorList[i].handleSpeed();

        // Compare motor speed and timer
        if(motorList[i].speed != 0 && (callbackTimer % motorList[i].speed) == 0) {
            // Handle motor rotation
            motorList[i].handleRotation();
        }
    }
    
    // Update callback timer
    callbackTimer = callbackTimer + TIMER_DELAY;

    // Check callback timer limits
    if(callbackTimer >= TIMER_COUNT_MAX) {
        callbackTimer = 0;
    }
}*/