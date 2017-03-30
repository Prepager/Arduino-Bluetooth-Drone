//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

/*
 * Function: isDebug
 * ----------------------------
 * Return weather or not the variable is in debug.
 *
 * @returns: bool
 */
bool isDebug(bool debugVariable) {
    return DEBUG && debugVariable;
}

/*
 * Function: debug
 * ----------------------------
 * Output string to Serial on debug.
 *
 * @returns: void
 */
void debug(String text) {
    if(DEBUG) {
        Serial.println(text);
    }
}