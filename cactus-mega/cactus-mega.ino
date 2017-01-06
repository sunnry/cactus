
// If you want to use HardwareSerial only, and not have to include SoftwareSerial support, you 
// can define SERIALCOMMAND_HARDWAREONLY in SerialCommand.h, which will cause it to build without
// SoftwareSerial support.   This makes the library act as it used to before SoftwareSerial 
// support was added, and you don't need this next include: 
//#include <SoftwareSerial.h>  

#include "SerialCommand.h"

#include <Wire.h>
#include "Kalman.h"
#include "MsTimer2.h"
#include "esc_control.h"
#include "ReadSensor.h"
#include "dht11.h"
#include "ServoControl.h"
#include "CurrentCheck.h"
#include "pwm_light.h"

//#define  PWM_LIGHT_TEST  1

#define  MSTIMER2_INTERVAL_LEVEL1  150
#define  MSTIMER2_INTERVAL_LEVEL2  1000

#ifdef PWM_LIGHT_TEST
int pwm_loop = 0;
#endif

#define RESTRICT_PITCH //comment out to restrict roll to +-90 deg instead

SerialCommand SCmd;   // The demo SerialCommand object

Kalman kalmanX; // Create the Kalman instances
Kalman kalmanY;
/* IMU Data */
double accX, accY, accZ;
double gyroX, gyroY, gyroZ;
int16_t tempRaw;

double gyroXangle, gyroYangle; // Angle calculate using the gyro only
double compAngleX, compAngleY; // Calculated angle using a complementary filter
double kalAngleX, kalAngleY; // Calculated angle using a Kalman filter

uint32_t timer;
uint8_t i2cData[14]; // Buffer for I2C data

//esc control handler
esc_control *escLeft ;  //pin7
esc_control *escRight;  //pin8

//sensor related
ReadSensor *sensors;
double batVoltage;

//dht11 related
dht11 DHT11;
float humidity;
float temperature;
int dht_access_interval;

//servo related
ServoControl *p35Servo;

//CurrentCheck related
CurrentCheck *currentCheck;
double mcuCurrent;

//pwm light related
PwmLight *lights;

//response pi3 the status data of rov
//STATUS0: response to pi3 about rov status
void InfoPiPIDStatus(){
  Serial1.print("STATUS0:");Serial1.print(kalAngleX);Serial1.print(",");Serial1.print(kalAngleY);Serial1.print(",");Serial1.print(batVoltage);Serial1.print(",");Serial1.print(humidity);Serial1.print(",");Serial1.print(temperature);Serial1.print(",");Serial1.println(mcuCurrent);
}

void ContinueInformPi3(){
  Serial1.println("RES00:OK");
}

void SetupSerialCommands(){
  //CMD0: balance self control mode.                       return RES0
  //CMD1: user control mode                                return RES1
  //CMD2: turn light on or off                             return RES2
  //CMD3: if receive this command, the Pi has been ok      return RES3
  //CMD4: set pwm light lumination                         return RES4
  //RES00:OK : info pi3 control board has ready
  //if received command can not been recognized, return 'UNKNOWN CMD' to pi3
  SCmd.addCommand("CMD0",onSelfBalance);    // rov self control mode
  SCmd.addCommand("CMD1",onUserControl);    // rov user control mode
  SCmd.addCommand("CMD2",onLedControl);     // use led to inform user board status
  SCmd.addCommand("CMD3",onServerStatus);
  SCmd.addCommand("CMD4",onPwmLightSet);
  SCmd.addDefaultHandler(unrecognized);  // Handler for command that isn't matched  (says "What?") 
  //send RES00 to pi3 to inform pi3 control board has been ok
  Serial1.println("RES00:OK");
  //if after 1s the pi3 still do not response,send RES00 again and until pi3 response will stop 
  MsTimer2::set(MSTIMER2_INTERVAL_LEVEL2,ContinueInformPi3);
  MsTimer2::start();
}

void setup()
{  
  Serial1.begin(115200); 
  Wire.begin();
#if ARDUINO >= 157
  Wire.setClock(400000UL); // Set I2C frequency to 400kHz
#else
  TWBR = ((F_CPU / 400000UL) - 16) / 2; // Set I2C frequency to 400kHz
#endif

  i2cData[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
  i2cData[2] = 0x00; // Set Gyro Full Scale Range to ±250deg/s
  i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g
  while (i2cWrite(0x19, i2cData, 4, false)); // Write to all four registers at once
  while (i2cWrite(0x6B, 0x01, true)); // PLL with X axis gyroscope reference and disable sleep mode

  while (i2cRead(0x75, i2cData, 1));
  if (i2cData[0] != 0x68) { // Read "WHO_AM_I" register
    Serial1.print(F("Error reading sensor"));
    while (1);
  }
  delay(100); // Wait for sensor to stabilize

  /* Set kalman and gyro starting angle */
  while (i2cRead(0x3B, i2cData, 6));
  accX = (i2cData[0] << 8) | i2cData[1];
  accY = (i2cData[2] << 8) | i2cData[3];
  accZ = (i2cData[4] << 8) | i2cData[5];

  // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
  // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
  // It is then converted from radians to degrees
#ifdef RESTRICT_PITCH // Eq. 25 and 26
  double roll  = atan2(accY, accZ) * RAD_TO_DEG;
  double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
  double roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
#endif

  kalmanX.setAngle(roll); // Set starting angle
  kalmanY.setAngle(pitch);
  gyroXangle = roll;
  gyroYangle = pitch;
  compAngleX = roll;
  compAngleY = pitch;
  timer = micros();

  //init esc
  //set LEFT_ESC_PIN and RIGHT_ESC_PIN to OUTPUT mode to prevent bother each other.
  pinMode(LEFT_ESC_PIN,OUTPUT);
  pinMode(RIGHT_ESC_PIN,OUTPUT);
  escLeft = new esc_control(LEFT_ESC_PIN);
  escRight = new esc_control(RIGHT_ESC_PIN);

  //first time read battery voltage
  batVoltage = sensors->ReadBatteryVoltage();

  //fist time read DHT11 data
  dht_access_interval = 0;
  int chk = DHT11.read(DHT11_DATA_PIN);
  if(chk == DHTLIB_OK){
    humidity = DHT11.humidity;
    temperature = DHT11.temperature;
  }

  //first run servo to self check servo
  p35Servo = new ServoControl(SERVO1_PIN_NUM);
  p35Servo->setServoPos(0);

  //check current status
  currentCheck = new CurrentCheck();
  mcuCurrent = currentCheck->CheckControlMCUPart();


  //pwm light handler
  lights = new PwmLight();
  
  //setup customer commands
  SetupSerialCommands();
}

void loop()
{
  /* Update all the values */
  while (i2cRead(0x3B, i2cData, 14));
  accX = ((i2cData[0] << 8) | i2cData[1]);
  accY = ((i2cData[2] << 8) | i2cData[3]);
  accZ = ((i2cData[4] << 8) | i2cData[5]);
  tempRaw = (i2cData[6] << 8) | i2cData[7];
  gyroX = (i2cData[8] << 8) | i2cData[9];
  gyroY = (i2cData[10] << 8) | i2cData[11];
  gyroZ = (i2cData[12] << 8) | i2cData[13];

  double dt = (double)(micros() - timer) / 1000000; // Calculate delta time
  timer = micros();

  // Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
  // atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
  // It is then converted from radians to degrees
#ifdef RESTRICT_PITCH // Eq. 25 and 26
  double roll  = atan2(accY, accZ) * RAD_TO_DEG;
  double pitch = atan(-accX / sqrt(accY * accY + accZ * accZ)) * RAD_TO_DEG;
#else // Eq. 28 and 29
  double roll  = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
  double pitch = atan2(-accX, accZ) * RAD_TO_DEG;
#endif

  double gyroXrate = gyroX / 131.0; // Convert to deg/s
  double gyroYrate = gyroY / 131.0; // Convert to deg/s

#ifdef RESTRICT_PITCH
  // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
  if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) {
    kalmanX.setAngle(roll);
    compAngleX = roll;
    kalAngleX = roll;
    gyroXangle = roll;
  } else
    kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter

  if (abs(kalAngleX) > 90)
    gyroYrate = -gyroYrate; // Invert rate, so it fits the restriced accelerometer reading
  kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);
#else
  // This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
  if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) {
    kalmanY.setAngle(pitch);
    compAngleY = pitch;
    kalAngleY = pitch;
    gyroYangle = pitch;
  } else
    kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter

  if (abs(kalAngleY) > 90)
    gyroXrate = -gyroXrate; // Invert rate, so it fits the restriced accelerometer reading
  kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
#endif

  gyroXangle += gyroXrate * dt; // Calculate gyro angle without any filter
  gyroYangle += gyroYrate * dt;
  //gyroXangle += kalmanX.getRate() * dt; // Calculate gyro angle using the unbiased rate
  //gyroYangle += kalmanY.getRate() * dt;

  compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll; // Calculate the angle using a Complimentary filter
  compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;

  // Reset the gyro angle when it has drifted too much
  if (gyroXangle < -180 || gyroXangle > 180)
    gyroXangle = kalAngleX;
  if (gyroYangle < -180 || gyroYangle > 180)
    gyroYangle = kalAngleY;

  //continue read battery voltage in loop
  batVoltage = sensors->ReadBatteryVoltage();


  //read dht11 value continue
  dht_access_interval ++;
  if(dht_access_interval >= 4000){
    dht_access_interval = 0;
    int chk = DHT11.read(DHT11_DATA_PIN);
    if(chk == DHTLIB_OK){
      humidity = DHT11.humidity;
      temperature = DHT11.temperature;
    }else{
      Serial1.println("dht11 read failed");
    }
  }

  //continue check current
  mcuCurrent = currentCheck->CheckControlMCUPart();


#ifdef  PWM_LIGHT_TEST
    if(pwm_loop <= 255){
      lights->setLightLumination(PWM_LIGHT_ONE,pwm_loop);
      lights->setLightLumination(PWM_LIGHT_TWO,pwm_loop);
      lights->setLightLumination(PWM_LIGHT_THREE,pwm_loop);
      pwm_loop ++;
    }

    if(pwm_loop == 255)
      pwm_loop = 0;
#endif

  //get command from serial and process command
  SCmd.readSerial();
}

void onSelfBalance()
{
  Serial1.println("RES0:OK"); 
}

void onUserControl()
{
  char *arg;  
  arg = SCmd.next();    // Get the next argument from the SerialCommand object buffer
  if (arg != NULL)      // As long as it existed, take it
  {
    //Serial1.println(arg); 
  }
  Serial1.println("RES1:OK");
}

//this functiion must cowork with hardware
void onLedControl()
{
  Serial1.println("RES2:OK");
}

void onServerStatus()
{
  //callback function when pi3 inform control board about pi3 status
  char *arg;
  arg = SCmd.next();
  if(arg != NULL){
    if(strcmp(arg,"OK") == 0){
      MsTimer2::stop();
      Serial1.println("RES3:OK");
      MsTimer2::set(MSTIMER2_INTERVAL_LEVEL1,InfoPiPIDStatus);
      MsTimer2::start();
    }
  }
}

void onPwmLightSet(){
  char *arg;
  arg = SCmd.next();
  if(arg != NULL){
    int ret = atoi(arg);
    if(ret >=0 && ret <= 255){
      lights->setLightLumination(PWM_LIGHT_ONE,ret);
      lights->setLightLumination(PWM_LIGHT_TWO,ret);
      lights->setLightLumination(PWM_LIGHT_THREE,ret);
      Serial1.println("RES4:OK");
    }
  }
}

// This gets set as the default handler, and gets called when no other command matches. 
void unrecognized()
{
  Serial1.println("UNKNOWN CMD"); //not known command 
}

