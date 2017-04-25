//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

/*
 * Section: Modules
 * ----------------------------
 * Toggle various modules used in the build.
 */

// Bluetooth
#define MODULE_BLUETOOTH true	// Toggle the retrieval and processing of bluetooth signals.
#define MOUDLE_DEADMAN false	// Toggle the dead man's switch preventing actions on disconnect.

// Gyroscope
#define MODULE_GYROSCOPE true	// Toggle the retrieval and processing of gyroscope readings used for balancing.

// Controller
#define MODULE_CONTROLLER true	// Toggle the setup and handling of the motors.
#define MODULE_BALANCING true	// Toggle the PID balancing system.
#define MODULE_MOVEMENT true	// Toggle the movement based on bluetooth signals.


/*
 * Section: Debugging
 * ----------------------------
 * Output various information to serial monitor.
 */

// General
#define DEBUG false             // Toogle all debugging tools below and output general actions.

// Execution
#define DEBUG_EXECUTION false   // Output the speed of the application.

// Bluetooth
#define DEBUG_BLUETOOTH true    // Output chars received from bluetooth app (Except X and Z).

// Gyroscope
#define DEBUG_GYRO false        // Output X, Y and Z gyroscope readings in Deg.
#define DEBUG_ACCEL false       // Output X, Y and Z accelerometer readings in G.
#define DEBUG_AXES false        // Output Roll and Pitch in Deg.

// Controller
#define DEBUG_CONTROLLER false  // Output next speed for each motor in ms.
#define DEBUG_SETPOINTS false   // Output setpoint values for PID controller.


/*
 * Section: Pins
 * ----------------------------
 * Set the pins used for the various modules.
 */

// Bluetooth
#define PIN_BLUETOOTH_TX 5      // The pin number used for the Bluetooth TX connection.
#define PIN_BLUETOOTH_RX 3      // The pin number used for the Bluetooth RX connection.

// Controller
#define PIN_FRONTLEFT 10        // The pin number used for the front left motor (ESC: Yellow Wire).
#define PIN_FRONTRIGHT 6        // The pin number used for the front right motor (ESC: White Wire).
#define PIN_BACKRIGHT 9         // The pin number used for the back right motor (ESC: Brown Wire).
#define PIN_BACKLEFT 11         // The pin number used for the back left motor (ESC: Red Wire).


/*
 * Section: Calibration & fine tuning
 * ----------------------------
 * Calibration & fine tuning of the various modules.
 */

// Readings: ((0.66+0.73+0.68+0.73+0.67+0.63+0.73+0.64+0.69+0.70+0.61)/11)*2

// PID Controller
#define CONTROLLER_PID_P 0.6*0.5    // The PID Proportional value (Now)
#define CONTROLLER_PID_I 0.9/2      // The PID Integral value (Past)
#define CONTROLLER_PID_D 0.9/8      // the PID Derivative value (Future)

// Gyroscope
#define GYRO_CALIBRATE true     // Toggle the calibration of the gyroscope on launch.
#define GYRO_READINGS 1000      // The amount of gyroscope readings used for calibration (3ms in between).
#define GYRO_UP_READINGS 250    // The amount of gyroscope readings used for axes calibration (3ms in between).

// Controller
#define ANGLE_MOVEMENT 4        // The angle the motors should accept on movement.
#define SPEED_MOVEMENT 5        // The ms the motors should increase on up down movement.
#define SPEED_MINIMAL 750       // The minimal speed for the motors to enable but not spin.
#define SPEED_HOVER 1200        // The base and start value for hovering.
#define SPEED_RANGE_MIN 750     // The minimum speed for the motors.
#define SPEED_RANGE_MAX 1500    // The maximum speed for the motors.

// Motors
#define MOTOR_FL_OFFSET 1       // The offset in percent for the front left motor.
#define MOTOR_FR_OFFSET 1       // The offset in percent for the front right motor.
#define MOTOR_BR_OFFSET 1.01    // The offset in percent for the back right motor.
#define MOTOR_BL_OFFSET 1       // The offset in percent for the back left motor.


/*
 * Section: Other
 * ----------------------------
 * Other settings not suitable for other categories.
 */

// Bluetooth
#define BLUETOOTH_SETUP false   // Toggle the setup mode of the bluetooth module.
#define BLUETOOTH_DEADMAN 3     // The required time between readings for dead man's switch.
#define BLUETOOTH_SERIAL true   // Use USB serial as bluetooth signal

// Math
#define PI 3.1415926535         // The value of PI used for the PID controller (10 decimal places).