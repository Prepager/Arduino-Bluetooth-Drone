//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

// Settings - Debug
#define DEBUG true
#define DEBUG_GYRO false
#define DEBUG_ACCEL false
#define DEBUG_BLUETOOTH true

// Settings - Bluetooth
#define BLUETOOTH_SERIAL false

// Includes
#include "GyroReceiver.h";

// Classes
GyroReceiver gyro;

/*
 * Function: setup
 * ----------------------------
 * Setup function for Arduino (Run once)
 *
 * @returns: void
 */
void setup() {
	// Begin serial in debugging
	if(DEBUG) {
		Serial.begin(9600);
		debug("-- Debugging initialized --");
		debug("");
	}

	// Setup Gyro module
	gyro.setup();
	debug("+ Gyro initialized.");

	// Setup Bluetooth module
	// X
	debug("+ Bluetooth initialized.");

	// Completed
	debug("");
	debug("-- Setup Completed --");
	debug("");
}

/*
 * Function: debug
 * ----------------------------
 * Output string to Serial on debug
 *
 * @returns: void
 */
void debug(String text) {
	if(DEBUG) {
		Serial.println(text);
	}
}

/*
 * Function: loop
 * ----------------------------
 * Loop function for Arduino (Run every tick)
 *
 * @returns: void
 */
void loop() {
	// Retrieve gyro readings
	gyro.retrieve(DEBUG && DEBUG_GYRO, DEBUG && DEBUG_ACCEL);
}