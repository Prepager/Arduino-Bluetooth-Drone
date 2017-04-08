//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

// Dependencies
#include "SoftwareSerial.h";

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

        bool debugBluetooth;

        int tx, rx;
        char serialCommand, command;
        long lastAlive;

        SoftwareSerial * bluetoothSerial;

        BluetoothReceiver(int pinTX, int pinRX, bool debug);
        void setup();

        void retrieve();
        bool isAlive();

};

/*
 * Function: BluetoothReceiver
 * ----------------------------
 * Initialize BluetoothReceiver class.
 *
 * @returns: void
 */
BluetoothReceiver::BluetoothReceiver(int pinTX, int pinRX, bool debug) {
    // Variables
    tx = pinTX;
    rx = pinRX;
    debugBluetooth = debug;

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
void BluetoothReceiver::setup() {
    // Reset alive
    lastAlive = millis();

    // Begin serial
    if(BLUETOOTH_SETUP) {
        bluetoothSerial->begin(38400);
    }else{
        bluetoothSerial->begin(9600);
    }
}

/*
 * Function: retrieve
 * ----------------------------
 * Retrieve Bluetooth command.
 *
 * @returns: void
 */
void BluetoothReceiver::retrieve() {
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

    // Serial to Bluetooth
    if(debugBluetooth && Serial.available()) {
        // Read Serial signal
        serialCommand = Serial.read();

        // Write to Bluetooth
        bluetoothSerial->write(serialCommand);
    }

    // Output debug command
    if(debugBluetooth && command != 'Z' && command != 'X') {
        Serial.print("Bluetooth Signal: ");
        Serial.println(String(command));
    }
}

/*
 * Function: isAlive
 * ----------------------------
 * Check bluetooth alive.
 *
 * @returns: bool
 */
bool BluetoothReceiver::isAlive() {
    // Check time
    return ((millis() - lastAlive) >= BLUETOOTH_ALIVE);
}