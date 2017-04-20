//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

// Dependencies
//#include "TimerOne.h";

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
        bool killed = false, killedAlive = false;

        float pastX = 0, pastY = 0, pastZ = 0;  // Integral
        float futureX, futureY, futureZ;        // Derivative
        float errorX, errorY, errorZ;           // Proportional

        float setpointX = 0, setpointY = 0, setpointZ = 0;
        float pastErrorX = 0, pastErrorY = 0, pastErrorZ = 0;

        /*String movementMethod[4][2] = {
            {"W", "A"},
            {"W", "D"},
            {"S", "A"},
            {"S", "D"}
        };*/

        SpeedController(int test);
        void setup();
        
        void handle();
        void handleMovement();
        void handleBalance();

        void pointX(float value);
        void pointY(float value);
        //static void callback();

};

/*
 * Function: SpeedController
 * ----------------------------
 * Initialize the SpeedController class.
 *
 * @returns: void
 */
SpeedController::SpeedController(int test) {
    // Variables
    debugSpeed = isDebug(DEBUG_CONTROLLER);
}

/*
 * Function: setup
 * ----------------------------
 * Setup motor controller class.
 *
 * @returns: void
 */
void SpeedController::setup() {
    // Update base speed offsets
    motorList[0].baseSpeed += MOTOR_FL_OFFSET;
    motorList[1].baseSpeed += MOTOR_FR_OFFSET;
    motorList[2].baseSpeed += MOTOR_BR_OFFSET;
    motorList[3].baseSpeed += MOTOR_BL_OFFSET;

    // Setup motors
    for(int i = 0; i < 4; i++) {
        motorList[i].setup();

        // Base speed without balancing
        if(!MODULE_BALANCING) {
            motorList[i].nextSpeed = motorList[i].baseSpeed;
        }
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
    // Handle movement
    if(MODULE_MOVEMENT) {
        handleMovement();
    }

    // Balance motors
    if(MODULE_BALANCING) {
        handleBalance();
    }

    // Loop through motors
    for(int i = 0; i < 4; i++) {
        // Debugging speed
        if(debugSpeed) {
            Serial.print(motorList[i].pos);
            Serial.print(" # ");
            Serial.print(motorList[i].nextSpeed);
            Serial.print("   ");

            if(motorList[i].pos == 3) {
                Serial.println("");
            }
        }

        // Handle motor speed
        motorList[i].handle();
    }
}

/*
 * Function: handleMovement
 * ----------------------------
 * Handle the movement of the motors.
 *
 * @returns: void
 */
void SpeedController::handleMovement() {
    // Read bluetooth signal
    float a = ANGLE_MOVEMENT;
    float s = SPEED_MOVEMENT;
    String c = String(bluetooth.command);
    char cmd = bluetooth.command;

    // Dead man's mode
    if(MOUDLE_DEADMAN) {
        // Kill on lost connection
        if(!killed && !bluetooth.isAlive()) {
            killed = true;
            killedAlive = true;
        }

        // Toggle when connection open
        if(killed && killedAlive && bluetooth.isAlive()) {
            killed = false;
            killedAlive = false;
        }
    }

    // Ignore non-movement related commands
    if(c == "Z" || c == "X") {
        return;
    }

    // Toggle kill mode
    if(killed) {
        killed = false;
        killedAlive = false;
    }

    // Check for kill command
    if(c == "Q") {
        killed = true;
        killedAlive = false;
    }

    // Loop through motors
    for(int i = 0; i < 4; i++) {
        // Check for up/down command
        if(c == "T" || c == "g") {
            motorList[i].baseSpeed = motorList[i].baseSpeed + SPEED_MOVEMENT;
            break;
        } else if(c == "t" || c == "G") {
            motorList[i].baseSpeed = motorList[i].baseSpeed - SPEED_MOVEMENT;
            break;
        }

        // Switch commands
        switch(cmd) {
            // Forward
            case 'W':
                pointX(a);
                pointY(a);
                break;
            case 'w':
                pointX(-a);
                pointY(-a);
                break;

            // Backwards
            case 'S':
                pointX(-a);
                pointY(-a);
                break;
            case 's':
                pointX(a);
                pointY(a);
                break;

            // Left
            case 'A':
                pointX(a);
                pointY(-a);
                break;
            case 'a':
                pointX(-a);
                pointY(a);
                break;

            // Right
            case 'D':
                pointX(a);
                pointY(-a);
                break;
            case 'd':
                pointX(-a);
                pointY(a);
                break;
        }

        // Loop through commands
        /*for(int k = 0; k < 3; k++) {
            // Retrieve command from method
            String cmd = movementMethod[i][k];

            // Save inverted string case state
            String cmdUpper = cmd;
            cmd.toLowerCase();

            // Check for command
            if(c == cmd || c == cmdUpper) {
                switch(i) {
                    // Motor 0
                    case 0:
                        pastX = 0; pastErrorX = 0;

                        if(c == cmd) { setpointX -= s; return; }
                        setpointX += s;
                        break;

                    // Motor 1
                    case 1:
                        pastY = 0; pastErrorY = 0;

                        if(c == cmd) { setpointY -= s; return; }
                        setpointY += s;
                        break;

                    // Motor 2
                    case 2:
                        pastX = 0; pastErrorX = 0;

                        if(c == cmd) { setpointX += s; return; }
                        setpointX -= s;
                        break;

                    // Motor 3
                    case 3:
                        pastY = 0; pastErrorY = 0;

                        if(c == cmd) { setpointY += s; return; }
                        setpointY -= s;
                        break;
                }

                break;
            }
        }*/

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
    // Read gyro readings
    float x = gyro.roll;
    float y = gyro.pitch;

    Serial.print("X: ");
    Serial.print(setpointX);
    Serial.print(" Y:");
    Serial.println(setpointY);

    // X Axis
    errorX = setpointX - x;
    pastX += errorX * executionTime;
    futureX = (errorX - pastErrorX) / executionTime;
    int xOutput = (CONTROLLER_PID_P * errorX) + (CONTROLLER_PID_I * pastX) + (CONTROLLER_PID_D * futureX);
    pastErrorX = errorX;

    // Y Axis
    errorY = setpointY - y;
    pastY += errorY * executionTime;
    futureY = (errorY - pastErrorY) / executionTime;
    int yOutput = (CONTROLLER_PID_P * errorY) + (CONTROLLER_PID_I * pastY) + (CONTROLLER_PID_D * futureY);
    pastErrorY = errorY;

    // Write speed to motor based on gyro position
    motorList[0].nextSpeed = motorList[0].baseSpeed + xOutput;
    motorList[1].nextSpeed = motorList[1].baseSpeed + -yOutput;
    motorList[2].nextSpeed = motorList[2].baseSpeed + -xOutput;
    motorList[3].nextSpeed = motorList[3].baseSpeed + yOutput;

    // Check for kill switch
    if(killed) {
        // Turn off motors
        for(int i = 0; i < 4; i++) {
            motorList[i].nextSpeed = SPEED_MINIMAL;
        }
    }
}

/*
 * Function: pointX
 * ----------------------------
 * Set the vaue of setpointX
 *
 * @returns: void
 */
void SpeedController::pointX(float value) {
    // Save value
    setpointX += value;

    // Reset past values
    pastX = 0; pastErrorX = 0;
}

/*
 * Function: pointY
 * ----------------------------
 * Set the vaue of setpointY
 *
 * @returns: void
 */
void SpeedController::pointY(float value) {
    // Save value
    setpointY += value;

    // Reset past values
    pastY = 0; pastErrorY = 0;
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