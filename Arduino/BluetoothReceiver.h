//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

// Dependencies
#include <SoftwareSerial.h>

/*
 * Class: BluetoothReceiver
 * ----------------------------
 * Receive signal from Gyro and convert it.
 *
 * @returns: class
 */
class BluetoothReceiver {

    // Public
    public:

        int tx, rx;
        char command;

        int aliveLength;
        long lastAlive;

        SoftwareSerial * bluetoothSerial;

        BluetoothReceiver(int pinTX, int pinRX);
        void setup(int aliveTime);

        void retrieve(bool debugBluetooth = false);
        bool isAlive();

    // Protected
    protected:

        //

};

/*
 * Function: BluetoothReceiver
 * ----------------------------
 * Initialize BluetoothReceiver class.
 *
 * @returns: void
 */
BluetoothReceiver::BluetoothReceiver(int pinTX, int pinRX) {
    // Save ports
    tx = pinTX;
    rx = pinRX;

    // Open software serial
    bluetoothSerial = new SoftwareSerial(pinTX, pinRX);
}

/*
 * Function: setup
 * ----------------------------
 * Setup Bluetooth serial.
 *
 * @returns: void
 */
void BluetoothReceiver::setup(int aliveTime = 2) {
    // Reset alive
    lastAlive = millis();
    aliveLength = aliveTime;

    // Begin serial
    bluetoothSerial->begin(9600);
}

/*
 * Function: retrieve
 * ----------------------------
 * Retrieve Bluetooth command.
 *
 * @returns: void
 */
void BluetoothReceiver::retrieve(bool debugBluetooth = false) {
    // Bluetooth available
    if(bluetoothSerial->available()) {
        // Read Bluetooth signal
        command = bluetoothSerial->read();

        // Alive command
        if(command == 'X') {
            lastAlive = millis();
        }
    }else{
        // Sleep signal
        command = 'Z';
    }

    // Debug
    if(debugBluetooth) {
        Serial.println(command);
    }
}

/*
 * Function: isAlive
 * ----------------------------
 * Check bluetooth alive
 *
 * @returns: bool
 */
bool BluetoothReceiver::isAlive() {
    // Check time
    return ((millis() - lastAlive) >= aliveLength);
}