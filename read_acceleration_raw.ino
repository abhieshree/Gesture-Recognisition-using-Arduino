/**************************************************************************/
/*Sensor - Accelerometer Adafruit_MMA8451
  Data Output - Serial print
  Data Collected - 15 tuples of (ax,ay,az) per motion
  Each motion corresponds to time series data - from the time motion is dected till 15 samples are collected
  Any sample at a time t corresponds to a tuple of (ax, ay, az) at that time t
*/
/**************************************************************************/

#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();
float ACCEL_THRESHOLD = 2;
void setup(void) {
  Serial.begin(9600);
  
  //Serial.println("Adafruit MMA8451 test!");
  

  if (! mma.begin()) {
    //Serial.println("Couldnt start");
    while (1);
  }
  mma.setRange(MMA8451_RANGE_2_G);
}

void loop() {
  // Read the 'raw' data in 14-bit counts
  mma.read();
 
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);

  //read acceleration values with calibration
  float ax = event.acceleration.x +0.3;
  float ay = event.acceleration.y +0.3;
  float az = event.acceleration.z -9.8;

  if (!motionDetected(ax, ay, az)) { // if no motion dected, pause for 10 ms and return to beginning of loop to check for motion again
        delay(10);
        return;
    }
 
  recordsamples(); //if motion dected, call function recordsamples() 
  //Serial.println();
  delay(2000); //wait for 2seconds and then go back to beginning of loop to look for the next motion
   
}


//function checks if the combination of acceleration in all three axes exceeds a threshold
bool motionDetected(float ax, float ay, float az) {
    if(abs(ax)> ACCEL_THRESHOLD || abs(ay)> ACCEL_THRESHOLD || abs(az)> ACCEL_THRESHOLD){
      return 1;
    }
    else{
      return 0;
    }
}

//function reads 15 samples and prints at 100ms intervals for each axis
 void recordsamples() {
 for (int i = 0; i < 20; i++) {
  mma.read();
  sensors_event_t event; 
  mma.getEvent(&event);
  float ax = event.acceleration.x +0.3;
  float ay = event.acceleration.y +0.3;
  float az = event.acceleration.z -9.8;
  
  Serial.print(ax); Serial.print(",");
  Serial.print(ay);Serial.print(",");
  Serial.print(az);
  Serial.println();
  
  delay(100);
  
 }
 }
 
