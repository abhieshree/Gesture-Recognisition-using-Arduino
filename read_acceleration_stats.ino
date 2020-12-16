/**************************************************************************/
/*Sensor - Accelerometer Adafruit_MMA8451
  Data Output - Serial print
  Data Collected - (min(ax), max(ax), mean(|ax|), min(ay), max(ay), mean(|ay|), min(az), max(az), mean(|az|) tuple per motion
  Each motion corresponds to time series data - from the time motion is dected till 15 samples are collected
  Any sample at a time t corresponds to a tuple of (ax, ay, az) at that time t
*/
/**************************************************************************/

#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();
const int ledPin = 13;
float ACCEL_THRESHOLD = 2; //definig a threshold value. If acceleration exceeds this value then motion is dected
float ax_min;
float ax_max;
float ax_mean;
float ay_min;
float ay_max;
float ay_mean;
float az_min;
float az_max;
float az_mean;
float ax_sum;
float ay_sum;
float az_sum;
 
void setup(void) {
  Serial.begin(9600);
  pinMode (ledPin , OUTPUT );
  
  //Serial.println("Adafruit MMA8451 test!");
  

  if (! mma.begin()) {
    //Serial.println("Couldnt start");
    while (1);
  }
  //Serial.println("MMA8451 found!");
  
  mma.setRange(MMA8451_RANGE_2_G);
  
  
}

void loop() {
  //Read the 'raw' data in 14-bit counts
  mma.read();
  sensors_event_t event; 
  mma.getEvent(&event);
  //read acceleration values with calibration
  float ax = event.acceleration.x +0.3;
  float ay = event.acceleration.y +0.3;
  float az = event.acceleration.z -9.8;

  if (!motionDetected(ax, ay, az)){ // if no motion dected, pause for 10 ms and return to beginning of loop to check for motion again
        delay(10);
        return;
    }
 
  recordsamples(); //if motion dected, call function recordsamples()
  Serial.println();
  delay(2000); //wait for 2seconds and then go back to beginning of loop to look for the next motion
  digitalWrite (ledPin , LOW ); //turn off the LED to indicate that device is ready to detect motion again
  
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

//function reads 15 samples at 100ms intervals and finally outputs (serial print) their statistics (mean, min and max) for each axis
 void recordsamples() {
  ax_min = 0;
  ax_max = 0;
  ax_sum = 0;
  ay_min = 0;
  ay_max = 0;
  ay_sum = 0;
  az_min = 0;
  az_max = 0;
  az_sum = 0;
  digitalWrite (ledPin , HIGH );  
 for (int i = 0; i < 15; i++) {
 
  
  mma.read();
  sensors_event_t event; 
  mma.getEvent(&event);
  //read acceleration values with calibration
  float ax = event.acceleration.x +0.3;
  float ay = event.acceleration.y +0.3;
  float az = event.acceleration.z -9.8;
  
  if(ax<ax_min)
  ax_min = ax;
  if(ay<ay_min)
  ay_min = ay;
  if(az<az_min)
  az_min = az;

  if(ax>ax_max)
  ax_max = ax;
  if(ay>ay_max)
  ay_max = ay;
  if(az>az_max)
  az_max = az;

  ax_sum = ax_sum+abs(ax);
  ay_sum = ay_sum+abs(ay);
  az_sum = az_sum+abs(az);
  
  delay(100);
  
 }
 ax_mean = ax_sum/15;
 ay_mean = ay_sum/15;
 az_mean = az_sum/15;
 
 Serial.print(ax_min); Serial.print(",");Serial.print(ax_max);Serial.print(",");Serial.print(ax_mean);Serial.print(",");
 Serial.print(ay_min); Serial.print(",");Serial.print(ay_max);Serial.print(",");Serial.print(ay_mean);Serial.print(",");
 Serial.print(az_min); Serial.print(",");Serial.print(az_max);Serial.print(",");Serial.print(az_mean);Serial.print(",");
 
 }
 
