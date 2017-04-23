//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

/*
 * Function: isDebug
 * ----------------------------
 * Return weather or not the variable is currently in debug.
 *
 * @returns: bool
 */
extern bool isDebug(bool debugVariable) {
    return DEBUG && debugVariable;
}

/*
 * Function: debug
 * ----------------------------
 * Output string to Serial on debug.
 *
 * @returns: void
 */
extern void debug(String text) {
    if(DEBUG) {
        Serial.println(text);
    }
}

/*
 * Function: delayCountdown
 * ----------------------------
 * Countdown delay.
 *
 * @returns: void
 */
void delayCountdown(int time) {
	if(DEBUG) {
		// Loop through reps
		int reps = time/1000;
		for(int i = 0; i < reps; i++) {
			// Out time left
			Serial.print("+ Starting in ");
			Serial.print(reps-i);
			Serial.println(" seconds.");

			// Delay 1 second
			delay(1000);
		}
	} else {
		// Normal delay
		delay(time);
	}
}