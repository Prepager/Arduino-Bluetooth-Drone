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

        bool killed = false, killedAlive = false;
        bool debugSetpoint, debugSpeed;

        float pastX = 0, pastY = 0; // Integral
        float futureX, futureY;     // Derivative
        float errorX, errorY;       // Proportional

        float setpointX = 0, setpointY = 0;
        float pastErrorX = 0, pastErrorY = 0;

        SpeedController(bool startKilled);
        void setup();
        
        void handle();
        void handleMovement();
        void handleBalance();

        void pointX(float value);
        void pointY(float value);

};

/*
 * Function: SpeedController
 * ----------------------------
 * Initialize the SpeedController class.
 *
 * @returns: void
 */
SpeedController::SpeedController(bool startKilled) {
    // Killed
    killed = startKilled;

    // Debugging
    debugSetpoint = isDebug(DEBUG_SETPOINTS);
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

        // Set base speed without balancing
        if(!MODULE_BALANCING) {
            motorList[i].nextSpeed = motorList[i].baseSpeed;
        }
    }
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
    // Retrieve bluetooth signal
    char cmd = bluetooth.command;

    // Check Dead man's switch
    if(MOUDLE_DEADMAN) {
        // Check for not alive and killed
        if(!killed && !bluetooth.isAlive()) {
            killed = true;
            killedAlive = true;
        }

        // Check for alive and killedAlive
        if(killedAlive && bluetooth.isAlive()) {
            killed = false;
            killedAlive = false;
        }
    }

    // Skip non-movement related commands
    if(cmd == 'Z' || cmd == 'X') {
        return;
    }

    // Check for killed and kill command
    if(cmd == 'Q') {
        killed = true;
        killedAlive = false;
    } else if(killed) {
        killed = false;
        killedAlive = false;
    }

    // Handle general speed movement
    for(int i = 0; i < 4; i++) {
        // Save current motor
        MotorController *curMotor = &motorList[i];

        // Handle UP/DOWN command
        if(cmd == 'T' || cmd == 'g') {
            // Increase speed
            curMotor->baseSpeed += SPEED_MOVEMENT;
            continue;
        } else if(cmd == 't' || cmd == 'G') {
            // Lower speed
            curMotor->baseSpeed -= SPEED_MOVEMENT;
            continue;
        }

        // Handle rotate commands
        if(curMotor->pos == 0 || curMotor->pos == 2) {
            // Check for left rotation
            if(cmd == 'F') {
                // Increase speed
                curMotor->baseSpeed += SPEED_MOVEMENT;
                continue;
            } else if(cmd == 'f') {
                // Lower speed
                curMotor->baseSpeed -= SPEED_MOVEMENT;
                continue;
            }
        } else if(curMotor->pos == 1 || curMotor->pos == 3) {
            // Check for right rotation
            if(cmd == 'H') {
                // Increase speed
                curMotor->baseSpeed += SPEED_MOVEMENT;
                continue;
            } else if(cmd == 'h') {
                // Lower speed
                curMotor->baseSpeed -= SPEED_MOVEMENT;
                continue;
            }
        }
    }

    // Handle x setpoint
    if(cmd == 's' || cmd == 'W' || cmd == 'A') {
        pointX(-ANGLE_MOVEMENT);
    } else if(cmd == 'S' || cmd == 'w' || cmd == 'a') {
        pointX(ANGLE_MOVEMENT);
    }

    // Handle y setpoint
    if(cmd == 'w' || cmd == 'S' || cmd == 'D') {
        pointY(ANGLE_MOVEMENT);
    } else if(cmd == 'W' || cmd == 's' || cmd == 'd') {
        pointY(-ANGLE_MOVEMENT);
    }

    // Output setpoint values
    if(debugSetpoint) {
        // Output Title
        Serial.print("Setpoint (deg) ");

        // SetPoint X
        Serial.print("X=");
        if(setpointX >= 0) { Serial.print(" "); }
        Serial.print(setpointX);

        // SetPoint Y
        Serial.print(" Y=");
        if(setpointY >= 0) { Serial.print(" "); }
        Serial.println(setpointY);
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