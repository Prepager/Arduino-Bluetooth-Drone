//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

// Prevent double inclusion
#pragma once

// Dependencies
#include "Wire.h";

/*
 * Class: GyroReceiver
 * ----------------------------
 * Receive signal from Gyro and convert it.
 *
 * @returns: class
 */
class GyroReceiver {

    // Public
    public:

        bool debugGyro, debugAccel;

        long accelX, accelY, accelZ;
        float gForceX, gForceY, gForceZ;

        long gyroX, gyroY, gyroZ;
        float rotX, rotY, rotZ;

        GyroReceiver(bool gyro = false, bool accel = false);
        void setup();

        void retrieve();
        void outputValues();

        void retrieveRawAccel();
        void processRawAccel();

        void retrieveRawGyro();
        void processRawGyro();
    
};

/*
 * Function: GyroReceiver
 * ----------------------------
 * Initialize GyroReceiver class.
 *
 * @returns: void
 */
GyroReceiver::GyroReceiver(bool gyro = false, bool accel = false) {
    // Debug
    debugGyro = gyro;
    debugAccel = accel;
}

/*
 * Function: setup
 * ----------------------------
 * Setup gyro and prepare for values.
 *
 * @returns: void
 */
void GyroReceiver::setup() {
    // Begin Wire
    Wire.begin();

    // Begin transmission to MPU address
    Wire.beginTransmission(0b1101000);

        // Access '6.B - Power Management' register
        Wire.write(0x6B);

        // Set 'Sleep' register to 0
        Wire.write(0b00000000);

        // End transmission
        Wire.endTransmission();

    // Begin transmission to MPU address
    Wire.beginTransmission(0b1101000);

        // Access '1.B - Gyroscope Configuration' register
        Wire.write(0x1B);

        // Set gyro scale to +/- 250deg./s
        Wire.write(0x00000000);

        // End transmission
        Wire.endTransmission();

    // Begin transmission to MPU address
    Wire.beginTransmission(0b1101000);

        // Access '1.C - Acccelerometer Configuration' register
        Wire.write(0x1C);
        
        // Set accel scale to +/- 2g
        Wire.write(0b00000000);

        // End transmission
        Wire.endTransmission();
}

/*
 * Function: retrieve
 * ----------------------------
 * retrieve gyro and accel values and return them.
 *
 * @returns: void
 */
void GyroReceiver::retrieve() {
    // Retrieve values
    retrieveRawAccel();
    retrieveRawGyro();

    // Debug
    if(debugGyro || debugAccel) {
        outputValues();
    }
}

/*
 * Function: retrieveRawAccel
 * ----------------------------
 * Retrieve raw accelerometer values.
 *
 * @returns: void
 */
void GyroReceiver::retrieveRawAccel() {
    // Begin transmission to MPU address
    Wire.beginTransmission(0b1101000);

        // Prepare accel readings
        Wire.write(0x3B);

        // End transmission
        Wire.endTransmission();

    // Request accel readings
    Wire.requestFrom(0b1101000, 6);

    // Wait for readings
    while(Wire.available() < 6);

    // Read raw values x, y and z
    accelX = Wire.read()<<8|Wire.read();
    accelY = Wire.read()<<8|Wire.read();
    accelZ = Wire.read()<<8|Wire.read();

    // Process results
    processRawAccel();
}

/*
 * Function: processRawAccel
 * ----------------------------
 * Process raw accelerometer values to g.
 *
 * @returns: void
 */
void GyroReceiver::processRawAccel() {
    // Divide with 16384 to convert raw +/- 2g range values to g.
    gForceX = accelX / 16384.0;
    gForceY = accelY / 16384.0; 
    gForceZ = accelZ / 16384.0;
}

/*
 * Function: retrieveRawGyro
 * ----------------------------
 * Retrieve raw gyro values.
 *
 * @returns: void
 */
void GyroReceiver::retrieveRawGyro() {
    // Begin transmission to MPU address
    Wire.beginTransmission(0b1101000);

        // Prepare gyro readings
        Wire.write(0x43);

        // End transmission
        Wire.endTransmission();

    // Request gyro readings
    Wire.requestFrom(0b1101000, 6);

    // Wait for readings
    while(Wire.available() < 6);

    // Read raw values x, y and z
    gyroX = Wire.read()<<8|Wire.read();
    gyroY = Wire.read()<<8|Wire.read();
    gyroZ = Wire.read()<<8|Wire.read();

    // Process results
    processRawGyro();
}

/*
 * Function: processRawGyro
 * ----------------------------
 * Process raw gyro values to deg.
 *
 * @returns: void
 */
void GyroReceiver::processRawGyro() {
    // Divide with 131 to convert raw +/- 250deg./s range values to deg.
    rotX = gyroX / 131.0;
    rotY = gyroY / 131.0; 
    rotZ = gyroZ / 131.0;
}

/*
 * Function: outputValues
 * ----------------------------
 * Output gyro and accelerometer values.
 *
 * @returns: void
 */
void GyroReceiver::outputValues() {
    // Output gyro values
    if(debugGyro)
    {
        Serial.print("Gyro (deg)");

            // Gyro X
            Serial.print(" X=");
            if(rotX >= 0) { Serial.print(" "); }
            Serial.print(rotX);

            // Gyro Y
            Serial.print(" Y=");
            if(rotY >= 0) { Serial.print(" "); }
            Serial.print(rotY);

            // Gyro Z
            Serial.print(" Z=");
            if(rotZ >= 0) { Serial.print(" "); }
            Serial.print(rotZ);

            // Only gyro
            if(!debugAccel) {
                Serial.println();
            }
    }

    // Output accelerometer values
    if(debugAccel)
    {
        if(debugGyro) {
            Serial.print(" Accel (g)");
        }else{
            Serial.print("Accel (g)");
        }

            // Accelerometer X
            Serial.print(" X=");
            if(gForceX >= 0) { Serial.print(" "); }
            Serial.print(gForceX);

            // Accelerometer Y
            Serial.print(" Y=");
            if(gForceY >= 0) { Serial.print(" "); }
            Serial.print(gForceY);

            // Accelerometer Z
            Serial.print(" Z=");
            if(gForceZ >= 0) { Serial.print(" "); }
            Serial.println(gForceZ);
    }
}