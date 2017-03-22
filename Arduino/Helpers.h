//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

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