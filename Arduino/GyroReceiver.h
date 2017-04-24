//----------------------------------------------\\
//  Teknifags Projekt: Eksamensprojekt - Drone  \\
//   Periode: Uge 9 (28 Feb) - Uge 17 (25 Apr)  \\
//        Andreas, Jesper og Marcus - 3C        \\
//----------------------------------------------\\

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

        bool debugAxes, debugGyro, debugAccel;

        long accelX, accelY, accelZ;
        float gForceX, gForceY, gForceZ;

        long gyroX, gyroY, gyroZ;
        long gyroXC, gyroYC, gyroZC;
        float rotX, rotY, rotZ;

        float roll, pitch;
        bool calibrateGyro;

        GyroReceiver(bool shouldCalibrate);

        void setup();
        void calibrate();

        void retrieve(bool output = true);
        void outputValues();

        void retrieveRawAccel();
        void processRawAccel();

        void retrieveRawGyro(bool calibrate = false);
        void processRawGyro();

        void processAxes();
    
};

/*
 * Function: GyroReceiver
 * ----------------------------
 * Initialize GyroReceiver class.
 *
 * @returns: void
 */
GyroReceiver::GyroReceiver(bool shouldCalibrate) {
    // Calibrate
    calibrateGyro = shouldCalibrate;

    // Debugging
    debugGyro = isDebug(DEBUG_GYRO);
    debugAccel = isDebug(DEBUG_ACCEL);
    debugAxes = isDebug(DEBUG_AXES);
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

    // Calibrate gyroscope
    if(calibrateGyro) {
        // Send calibration warning
        Serial.println("+ Gyro calibration starting in 2 seconds.");
        delay(2000);

        // Calibrate Gyro
        calibrate();
        Serial.println("+ Gyro calibrated.");
    }

    // Calibrate axes readings
    for(int i = 0; i < GYRO_UP_READINGS; i++) {
        // Set new execution time used for degrees convertion
        executionTime = 0.031;

        // Retrieve gyro values
        retrieve(false);

        // Delay for better values
        delay(3);
    }
}

/*
 * Function: retrieve
 * ----------------------------
 * retrieve gyro and accel values and return them.
 *
 * @returns: void
 */
void GyroReceiver::retrieve(bool output = true) {
    // Retrieve values
    retrieveRawAccel();
    retrieveRawGyro();

    // Process axes
    processAxes();

    // Debug
    if(output && (debugGyro || debugAccel || debugAxes)) {
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
void GyroReceiver::retrieveRawGyro(bool calibrate = false) {
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
    if(!calibrate)
    {
        // Calibrate
        gyroX -= gyroXC;
        gyroY -= gyroYC;
        gyroZ -= gyroZC;

        // Process
        processRawGyro();
    }
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
 * Function: processAxes
 * ----------------------------
 * Process gyro and accel data to retrieve roll and pitch.
 *
 * @returns: void
 */
void GyroReceiver::processAxes() {
    // Convert degrees/s to degrees
    pitch += rotX * executionTime;
    roll -= rotY * executionTime;

    // Calculate total accel force
    int totalForce = abs(accelX) + abs(accelY) + abs(accelZ);

    // Compensate for gyro error
    // 16Bit: 8192 = 0.5G; 32768 = 2.0G
    if(totalForce > 8192 && totalForce < 32768) {
        // Pitch
        float pitchAccel = atan2f(accelY, accelZ) * (180 / PI);
        pitch = (pitch * 0.97) + (pitchAccel * 0.03);

        // Roll
        float pitchRoll = atan2f(accelX, accelZ) * (180 / PI);
        roll = (roll * 0.97) + (pitchRoll * 0.03);
    }
}

/*
 * Function: calibrate
 * ----------------------------
 * Calibrate gyro values.
 *
 * @returns: void
 */
void GyroReceiver::calibrate() {
    // Perform calibration readings
    for(int i = 0; i < GYRO_READINGS; i++) {
        // Retrieve raw gyro readings
        retrieveRawGyro(true);

        // Add radings to average
        gyroXC += gyroX;
        gyroYC += gyroY;
        gyroZC += gyroZ;

        // Delay next reading
        delay(3);
    }

    // Save readings to calibrate var
    gyroXC /= GYRO_READINGS;
    gyroYC /= GYRO_READINGS;
    gyroZC /= GYRO_READINGS;
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

    // Output axes values
    if(debugAxes)
    {
        // Roll
        Serial.print("Roll (Deg) ");
        Serial.print(roll);

        // Pitch
        Serial.print(" Pitch (Deg) ");
        Serial.println(pitch);
    }
}