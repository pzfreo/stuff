#include <Button.h>



//-------------------------------------------------------------------
//
// Sous Vide Controller
// Bill Earl - for Adafruit Industries
//
// Based on the Arduino PID and PID AutoTune Libraries
// by Brett Beauregard
//------------------------------------------------------------------
#define SETPOINT 58
#define KP 177
#define KI .07
#define KD .0



// PID Library
#include <PID_v1.h>



#include <SPI.h> // We'll use SPI to transfer data. Faster!

#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 8

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

#define PIN_SCE   12
#define PIN_DC    10
#define PIN_SDIN  11
#define PIN_SCLK  13


#define LCD_C     LOW
#define LCD_D     HIGH

#define LCD_X     84
#define LCD_Y     48
/* Font table:
  This table contains the hex values that represent pixels for a
  font that is 5 pixels wide and 8 pixels high. Each byte in a row
  represents one, 8-pixel, vertical column of a character. 5 bytes
  per character. */

static const byte ASCII[][5] =
{
  {0x00, 0x00, 0x00, 0x00, 0x00} // 20
  , {0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
  , {0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
  , {0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
  , {0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
  , {0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
  , {0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
  , {0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
  , {0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
  , {0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
  , {0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
  , {0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
  , {0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
  , {0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
  , {0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
  , {0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
  , {0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
  , {0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
  , {0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
  , {0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
  , {0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
  , {0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
  , {0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
  , {0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
  , {0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
  , {0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
  , {0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
  , {0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
  , {0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
  , {0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
  , {0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
  , {0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
  , {0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
  , {0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
  , {0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
  , {0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
  , {0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
  , {0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
  , {0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
  , {0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
  , {0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
  , {0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
  , {0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
  , {0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
  , {0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
  , {0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
  , {0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
  , {0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
  , {0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
  , {0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
  , {0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
  , {0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
  , {0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
  , {0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
  , {0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
  , {0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
  , {0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
  , {0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
  , {0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
  , {0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
  , {0x02, 0x04, 0x08, 0x10, 0x20} // 5c ¥
  , {0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
  , {0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
  , {0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
  , {0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
  , {0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
  , {0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
  , {0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
  , {0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
  , {0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
  , {0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
  , {0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
  , {0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
  , {0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
  , {0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
  , {0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
  , {0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
  , {0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
  , {0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
  , {0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
  , {0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
  , {0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
  , {0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
  , {0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
  , {0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
  , {0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
  , {0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
  , {0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
  , {0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
  , {0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
  , {0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
  , {0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
  , {0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
  , {0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
  , {0x10, 0x08, 0x08, 0x10, 0x08} // 7e ←
  , {0x78, 0x46, 0x41, 0x46, 0x78} // 7f →
};



// ***************
// PIMOTE
// ****************


int gpio17 = 5; // Encoded Signal D0
int gpio22 = 4; // Encoder Signal D1
int gpio23 = 3; // Encoder Signal D2
int gpio27 = 2; // Encoder Signal D3
int gpio24 = 6; // MODSEL mode select signal (OOK/FSK)
int gpio25 = 7; // CE modular enable (Output ON/OFF)

// Socket     all      1       2      3       4
char* on[] = {"1011", "1111", "1110", "1101", "1100"};
char* off[] = {"0011", "0111", "0110", "0101", "0100"};

void setupPimotePins()
{
  // Select the pins used for the encoder K0-K3 data inputs
  pinMode(gpio17, OUTPUT);
  pinMode(gpio22, OUTPUT);
  pinMode(gpio23, OUTPUT);
  pinMode(gpio27, OUTPUT);

  // Select the signal used to select ASK/FSK
  pinMode(gpio24, OUTPUT);
  // Select the signal used to enable/disable the modulator
  pinMode(gpio25, OUTPUT);

  // Disable the modulator by setting CE pin lo
  digitalWrite(gpio25, LOW);

  // Set the modulator to ASK for On Off Keying
  // by setting MODSEL pin lo
  digitalWrite(gpio24, LOW);

  // Initialise K0-K3 inputs of the encoder to 0000
  digitalWrite(gpio17, LOW);
  digitalWrite(gpio22, LOW);
  digitalWrite(gpio23, LOW);
  digitalWrite(gpio27, LOW);

  // Give a short delay after setup
  delay(500);
}

void state(int state, char* on_or_off[])
{
  digitalWrite(gpio27, LOW);
  digitalWrite(gpio23, LOW);
  digitalWrite(gpio22, LOW);
  digitalWrite(gpio17, LOW);

  if (on_or_off[state][0] - '0' == 1 )
    digitalWrite(gpio27, HIGH);

  if (on_or_off[state][1] - '0' == 1 )
    digitalWrite(gpio23, HIGH);


  if (on_or_off[state][2] - '0' == 1 )
    digitalWrite(gpio22, HIGH);


  if (on_or_off[state][3] - '0' == 1 )
    digitalWrite(gpio17, HIGH);

  delay(100);
  digitalWrite(gpio25, HIGH);
  delay(250);
  digitalWrite(gpio25, LOW);
}


void switch_on(int socket)
{
  state(socket, on);
}


void switch_off(int socket)
{
  state(socket, off);
}


// ************************************************
// PID Variables and constants
// ************************************************

//Define Variables we'll be connecting to
double Setpoint = SETPOINT;
double Input;
double Output;

volatile long onTime = 0;

// pid tuning parameters
double Kp;
double Ki;
double Kd;

// EEPROM addresses for persisted data
const int SpAddress = 0;
const int KpAddress = 8;
const int KiAddress = 16;
const int KdAddress = 24;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// 10 second Time Proportional Output window
int WindowSize = 10000;
unsigned long windowStartTime;



// ************************************************
// DiSplay Variables and constants
// ************************************************

const int logInterval = 10000; // log every 10 seconds
long lastLogTime = 0;

long start = millis();
double lastTemp;

Button b0 = Button(A0, true, false, 100);
Button b1 = Button(A1, true, false, 100);
Button b2 = Button(A2, true, false, 100);
Button b3 = Button(A3, true, false, 100);

long resetTime = millis();
enum State { normal, showstats, settemp, setp, seti, setd };

long totaltemps = 0;
long readings = 0;
long sqvariance = 0;
double max_t = 0;
double min_t = 1000;
double mean_t = 0;
double sd_t = 0;

void resetStats() {
  resetTime = millis();
  totaltemps = 0;
  readings = 0;
  sqvariance = 0;
  max_t = 0;
  min_t = 1000;
  mean_t = 0;
  sd_t - 0;
}

State dispstate = normal;
long screentimer = 0;

void setup()
{
  Serial.begin(9600);
  LcdInitialise();
  LcdClear();
  //         123456789012
  LcdString(" Welcome to  PZF Duino                testing     Energenie                          ");

  setupPimotePins();

  switch_off(1);
  delay(1000);
  switch_on(1);
  delay(1000);
  switch_off(4);


  Kp = KP;
  Ki = KI;
  Kd = KD;


  myPID.SetTunings(Kp, Ki, Kd);
  sensors.requestTemperatures(); // Send the command to get temperature
  Input = sensors.getTempCByIndex(0);
  lastTemp = Input;


  view();

  delay(1000);
  myPID.SetSampleTime(1000);
  myPID.SetOutputLimits(0, WindowSize);
  sensors.requestTemperatures(); // Send the command to get temperature

  myPID.SetMode(AUTOMATIC);
  windowStartTime = millis();
  //   StartAutoTune();



  // Run timer2 interrupt every 15 ms
  TCCR2A = 0;
  TCCR2B = 1 << CS22 | 1 << CS21 | 1 << CS20;

  //Timer2 Overflow Interrupt Enable
  TIMSK2 |= 1 << TOIE2;
}

// ************************************************
// Timer Interrupt Handler
// ************************************************
SIGNAL(TIMER2_OVF_vect)
{
  DriveOutput();
}

// ************************************************
// Main Control Loop
//
// All state changes pass through here
// ************************************************

boolean poweron = false;

int nextButton = -1;

void loop()
{
  //  Serial.print("loop: ");
  //  Serial.print(poweron ? "ON " : "OFF ");
  //  Serial.print(" ");
  //  Serial.println(WindowSize);

  // if screen is still not back to normal after 60 seconds revert
  if (dispstate == normal) {
    screentimer = 0;
  } else if (screentimer == 0) {
    screentimer = millis();
  } else if (millis() > screentimer + 60000) {
    screentimer = 0;
    dispstate = normal;
  }



  myPID.SetTunings(Kp, Ki, Kd);
  // Read the input:

  Input = sensors.getTempCByIndex(0);


  //  if (lastTemp != Input) {
  //
  //    Serial.print(millis() - start);
  //    Serial.print(',');
  //    double pct = map(Output, 0, WindowSize, 0, 1000);
  //    Serial.print(pct);
  //    Serial.print(',');
  //    Serial.println(Input);
  //    lastTemp = Input;
  //
  //  }
  //

  // stats
  if (Input > max_t) max_t = Input;
  if (Input < min_t) min_t = Input;

  totaltemps += Input;
  readings++;
  mean_t = (double) (totaltemps / readings);
  double v = Input - mean_t;
  sqvariance += (v * v);
  sd_t = (double) sqvariance / readings;

  sensors.requestTemperatures(); // prime the pump for the next one - but don't wait

  myPID.Compute();

  // Time Proportional relay state is updated regularly via timer interrupt.
  //  Serial.println(Output);
  //DriveOutput();
  onTime = Output;

  view();

  if (poweron) {
    switch_on(1);
  }
  else {
    switch_off(1);
  }



  if (nextButton != -1) {
    //    Serial.println(nextButton);
    switch (dispstate) {
      case normal:
        switch (nextButton) {
          case 0:
            dispstate = showstats;
            break;

          default:
            break;
        }
        break;
      case showstats:
        switch (nextButton) {
          case 0:
            dispstate = settemp;
            break;
          case 3:
            resetStats();
            break;
          default:
            break;
        }
        break;
      case settemp:
        switch (nextButton) {
          case 0:
            dispstate = setp;
            break;
          case 1:
            Setpoint += 0.5;
            break;
          case 2:
            Setpoint -= 0.5;
            break;
          case 3:
            dispstate = normal;
            break;
        }
        break;
      case setp:
        switch (nextButton) {
          case 0:
            dispstate = seti;
            break;
          case 1:
            Kp += 1;
            break;
          case 2:
            Kp -= 1;
            break;
          case 3:
            dispstate = normal;
            break;
        }
        break;
      case seti:
        switch (nextButton) {
          case 0:
            dispstate = setd;
            break;
          case 1:
            Ki += .1;
            break;
          case 2:
            Ki -= .1;
            break;
          case 3:
            dispstate = normal;
            break;
        }
        break;

      case setd:
        switch (nextButton) {
          case 0:
            dispstate = normal;
            break;
          case 1:
            Kd += .1;
            break;
          case 2:
            Kd -= .1;
            break;
          case 3:
            dispstate = normal;
            break;
        }
        break;
    }
    nextButton = -1;
  }


  //  delay(100);



}


// ************************************************
// Called by ISR every 15ms to drive the output
// ************************************************
void DriveOutput()
{

  b0.read();
  b1.read();
  b2.read();
  b3.read();
  if (b0.wasPressed()) nextButton = 0;
  if (b1.wasPressed()) nextButton = 1;
  if (b2.wasPressed()) nextButton = 2;
  if (b3.wasPressed()) nextButton = 3;

  long now = millis();
  // Set the output
  // "on time" is proportional to the PID output
  if (now - windowStartTime > WindowSize)
  { //time to shift the Relay Window
    windowStartTime += WindowSize;
  }
  if ((onTime > 100) && (onTime > (now - windowStartTime)))
  {
    poweron = true;
  }
  else
  {
    poweron = false;
  }

}



int isRight()
{
  if (abs(Input - Setpoint) > 1.0)
  {
    return 1;
  }
  else if (abs(Input - Setpoint) > 0.2)
  {
    return -1;
  }
  else
  {
    return 0;
  }
}

#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)




void view()
{


  //  LcdClear();
  char line[73];
  for (int i = 0; i < 73; i++) {
    line[i] = ' ';
  }

  switch (dispstate) {

    case normal: {
        //            0         1         2         3         4         5         6         7
        //            012345678901234567890123456789012345678901234567890123456789012345678901
        memcpy(line, "T: xxxx.xx  R: xxxx.xx  M: xxxx.xx  O: xxxx.xx", 45);
        char buff[10];
        dtostrf(Input, 7, 2, buff);
        memcpy(line + 3, buff, 7);
        dtostrf(Setpoint, 7, 2, buff);
        memcpy(line + 15, buff, 7);
        dtostrf(mean_t, 7, 2, buff);
        memcpy(line + 27, buff, 7);
        double pct = map(Output, 0, WindowSize, 0, 1000);
        dtostrf(pct, 7, 2, buff);
        memcpy(line + 39, buff, 7);

        break;
      }

    case showstats: {
        //            0         1         2         3         4         5         6         7
        //            012345678901234567890123456789012345678901234567890123456789012345678901
        memcpy(line, "CT:xxxx.xx  AV:xxxx.xx  MN:xxxx.xx  MX:xxxx.xx  SD:xxxx.xx  ", 60);
        char buff[10];
        dtostrf(Input, 7, 2, buff);
        memcpy(line + 3, buff, 7);
        dtostrf(mean_t, 7, 2, buff);
        memcpy(line + 15, buff, 7);
        dtostrf(min_t, 7, 2, buff);
        memcpy(line + 27, buff, 7);
        
        dtostrf(max_t, 7, 2, buff);
        memcpy(line + 39, buff, 7);
        dtostrf(sd_t, 7, 2, buff);
        memcpy(line + 51, buff, 7);

        break;
      }

    case settemp: {
        //            0         1         2         3         4         5         6         7
        //            012345678901234567890123456789012345678901234567890123456789012345678901
        memcpy(line, "T: xxxx.xx  R: xxxx.xx  ", 24);
        char buff[10];
        dtostrf(Input, 7, 2, buff);
        memcpy(line + 3, buff, 7);
        dtostrf(Setpoint, 7, 2, buff);
        memcpy(line + 15, buff, 7);
        break;
      }
    case setp: {
        
        memcpy(line, "P: xxxx.xx  ", 12);
        char buff[10];
        dtostrf(Kp, 7, 2, buff);
        memcpy(line + 3, buff, 7);
        break;
      }
    case seti: {
        memcpy(line, "I: xxxx.xx  ", 12);
        char buff[10];
        dtostrf(Ki, 7, 2, buff);
        memcpy(line + 3, buff, 7);
        break;
      }

    case setd: {
        memcpy(line, "D: xxxx.xx  ", 12);
        char buff[10];
        dtostrf(Kd, 7, 2, buff);
        memcpy(line + 3, buff, 7);
        break;
      }
  }

  long val = (millis() - resetTime) / 1000;
  int hours = numberOfHours(val);
  int minutes = numberOfMinutes(val);
  int seconds = numberOfSeconds(val);
  char buff[4];
  memcpy(line + 60, "ET:xxx:xx:xx", 12);
  dtostrf(hours, 3, 0, buff);
  if (hours < 10)  buff[1] = '0';
  memcpy(line + 63, buff, 3);
  dtostrf(minutes, 2, 0, buff);
  if (minutes < 10)  buff[0] = '0';
  memcpy(line + 67, buff, 2);
  dtostrf(seconds, 2, 0, buff);
  if (seconds < 10) buff[0] = '0';

  memcpy(line + 70, buff, 2);
  



  LcdString(line);

}

void LcdCharacter(char character)
{
  LcdWrite(LCD_D, 0x00);
  for (int index = 0; index < 5; index++)
  {
    LcdWrite(LCD_D, ASCII[character - 0x20][index]);
  }
  LcdWrite(LCD_D, 0x00);
}

void LcdClear(void)
{

  for (int index = 0; index < LCD_X * LCD_Y / 8; index++)
  {
    LcdWrite(LCD_D, 0x00);
  }
}

void LcdInitialise(void)
{
  pinMode(PIN_SCE, OUTPUT);
  //  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_SDIN, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);
  //  digitalWrite(PIN_RESET, LOW);
  //  digitalWrite(PIN_RESET, HIGH);
  LcdWrite(LCD_C, 0x21 );  // LCD Extended Commands.
  LcdWrite(LCD_C, 0xB1 );  // Set LCD Vop (Contrast).
  LcdWrite(LCD_C, 0x04 );  // Set Temp coefficent. //0x04
  LcdWrite(LCD_C, 0x14 );  // LCD bias mode 1:48. //0x13
  LcdWrite(LCD_C, 0x20 );  // LCD Basic Commands
  LcdWrite(LCD_C, 0x0C );  // LCD in normal mode.
}

void LcdString(char *characters)
{

  boolean s = true;
  for (int i = 0; i < 72; i++)
  {
    if (characters[i] == 0) {
      LcdCharacter(' ');
    }
    else {


      LcdCharacter(characters[i]);
    }


  }
}

void LcdWrite(byte dc, byte data)
{
  digitalWrite(PIN_DC, dc);
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  digitalWrite(PIN_SCE, HIGH);
}
