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
        char command;
        long lastAlive;

        SoftwareSerial * bluetoothSerial;

        BluetoothReceiver(int pinTX, int pinRX);
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
BluetoothReceiver::BluetoothReceiver(int pinTX, int pinRX) {
    // Save TX and RX transfer ports
    tx = pinTX;
    rx = pinRX;

    // Set debugging mode
    debugBluetooth = isDebug(DEBUG_BLUETOOTH);

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
    // Reset alive timer
    lastAlive = millis();

    // Begin serial connection
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
    // Check if Bluetooth signal is available
    if(bluetoothSerial->available()) {
        // Read Bluetooth signal
        command = bluetoothSerial->read();
    }else{
        // Return sleep signal
        command = 'Z';
    }

    // Check if serial is used for bluetooth
    if(BLUETOOTH_SERIAL && Serial.available()) {
        // Read serial signal
        char serialCommand = Serial.read();

        // Check for serial line endings
        if(serialCommand != '\n' && serialCommand != '\r') {
            command = serialCommand;
        }
    }

    // Reset alive timer
    if(command != 'Z') {
        lastAlive = millis();
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
    // Return true if alive timer is below limit
    return ((millis() - lastAlive) >= BLUETOOTH_DEADMAN);
}