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
    motorBackLeft, motorBackRight
};

extern int debugList[4] = {
    8, 7,
    12, 4
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

        bool debugSpeed, launched, balancing;
        long balancingDone;
        bool changed[4];

        SpeedController(bool debug = false);
        void setup();
        
        void handle();
        void handleBalance();
        void balanceStep(bool step, bool equal, float reading, int motor, int inv, int invEqual);
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

        // DEBUG
        pinMode(debugList[i], OUTPUT);
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
    // Check launched status
    if(!launched) {
        // Gyro above launch force limit
        if(gyro.rotY > CTLR_LAUNCH_FORCE && !balancing) {
            // Save status
            balancing = true;
            balancingDone = millis() + CTLR_BALANCE_TIMER;

            // Save motor speed
            for(int i = 0; i < 4; i++) {
                // Set new balanced speed
                int curSpeed = motorList[i].speed;
                motorList[i].baseSpeed = curSpeed;
                motorList[i].nextSpeed = curSpeed;

                // Output debug message
                if(debugSpeed) {
                    Serial.print("+ Motor (");
                    Serial.print(motorList[i].pos);
                    Serial.print(") stopped at ");
                    Serial.print(curSpeed);
                    Serial.println(" for launch.");
                }
            }

            // Output debug message
            if(debugSpeed) {
                Serial.print("");
                Serial.println("+ Motors waiting for balancing.");
            }
        }

        // Gyro launched and balanced
        if(balancing && millis() >= balancingDone) {
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
        }
    }

    // Balance motors
    handleBalance();

    // Handle motor speed
    for(int i = 0; i < 4; i++) {
        motorList[i].handle();
    }
}

/*
 * Function: balanceStep
 * ----------------------------
 * Handle each step of the balance process.
 *
 * @returns: void
 */
void SpeedController::balanceStep(bool step, bool equal, float reading, int motor, int inv, int invEqual) {
    if(step) {
        // DEBUG LEDS
        digitalWrite(debugList[motor], HIGH);

        // Check for inverse action
        if(motorList[motor].baseSpeed <= CTLR_INVERSE_SPEED) {
            // Increase current motor
            motorList[motor].balanceSpeed(reading);
            changed[motor] = true;
        } else if(!equal) {
            // Decrease opposite motor
            motorList[inv].balanceSpeed(reading, true);
            changed[inv] = true;
        } else {
            // Decrease opposit motor when equal (2 active)
            motorList[invEqual].balanceSpeed(reading, true);
            changed[invEqual] = true;
        }
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
    // Read accel readings
    float x = gyro.gForceX;
    float y = gyro.gForceY;
    float z = gyro.gForceZ;
    float f = CTLR_BALANCE_FORCE;

    // Handle Y axis
    bool left = (y > f);
    bool right = (y < -f);

    // Handle Z axis
    bool backwards = (z > f);
    bool forward = (z < -f);

    // DEBUG LEDS
    for(int i = 0; i < 4; i++) {
        digitalWrite(debugList[i], LOW);
    }

    // Reset changed
    for(int i = 0; i < 4; i++) {
        changed[i] = false;
    }

    // Y Axis
    balanceStep((left && !backwards), (!forward && !backwards), y, 0, 2, 1);
    balanceStep((left && !forward), (!forward && !backwards), y, 2, 0, 3);
    balanceStep((right && !backwards), (!forward && !backwards), y, 1, 3, 0);
    balanceStep((right && !forward), (!forward && !backwards), y, 3, 1, 2);

    // Z Axis
    balanceStep((forward && !right), (!left && !right), z, 0, 1, 2);
    balanceStep((forward && !left), (!left && !right), z, 1, 0, 3);
    balanceStep((backwards && !right), (!left && !right), z, 2, 3, 0);
    balanceStep((backwards && !left), (!left && !right), z, 3, 2, 1);

    // Loop though and check unsaved
    for(int i = 0; i < 4; i++) {
        if(!changed[i] && motorList[i].baseSpeed != 1) {
            // Set speed to base
            motorList[i].nextSpeed = motorList[i].baseSpeed;
        }
    }
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