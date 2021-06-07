//Declare pin functions on Redboard
#define stp 2
#define dir 3
#define MS1 4
#define MS2 5
#define EN  6
#define stp_2 21
#define dir_2 20
#define MS1_2 19
#define MS2_2 18
#define EN_2  17
#define BTN_RED 7
#define BTN_BLK 8
#define PIN_IN1 10
#define PIN_IN2 9
#define BTN_ENC 11

#include <RotaryEncoder.h>

// Setup a RotaryEncoder with 4 steps per latch for the 2 signal input pins:
static RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::FOUR3);
static int encoderPosition = 0;

//Declare variables for functions
int menu_pos = 0;
String menu[4] = {"Powrot", "Grubosc drutu", "Predkosc", "Sterowanie reczne"};

void setup() {
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(stp_2, OUTPUT);
  pinMode(dir_2, OUTPUT);
  pinMode(MS1_2, OUTPUT);
  pinMode(MS2_2, OUTPUT);
  pinMode(EN_2, OUTPUT);
  pinMode(BTN_RED, INPUT_PULLUP);
  pinMode(BTN_BLK, INPUT_PULLUP);
  pinMode(BTN_ENC, INPUT_PULLUP);
  resetEDPins(); //Set step, direction, microstep and enable pins to default states
  Serial.begin(9600); //Open Serial connection for debugging
  //while (! Serial);
}

//Main loop
void loop() {
  //Wind the coil bacward
  while (digitalRead(BTN_RED) == LOW && menu_pos == 0) {
    digitalWrite(EN, LOW); //Pull enable pin low to allow motor control
    digitalWrite(EN_2, LOW); //Pull enable pin low to allow motor control
    StepBackward();
    WindTheCoil();
  }
  //Wind the coil forward
  while (digitalRead(BTN_BLK) == LOW && menu_pos == 0) {
    digitalWrite(EN, LOW); //Pull enable pin low to allow motor control
    digitalWrite(EN_2, LOW); //Pull enable pin low to allow motor control
    StepForward();
    WindTheCoil();
  }
  //Enter menu
  while (digitalRead(BTN_ENC) == LOW) {
    menu_pos = 1;
    Serial.println("OK");
    if(isEncoderPositionChanged()){
      Serial.println("Działa");
    }
  }
  while (digitalRead(BTN_RED) == HIGH && digitalRead(BTN_BLK) == HIGH && digitalRead(BTN_ENC) == HIGH && menu_pos == 0) {
    delay(25);
    resetEDPins();
  }
}

//Reset Easy Driver pins to default states
void resetEDPins()
{
  //Conroller 1
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(EN, HIGH);
  //Controller 2
  digitalWrite(stp_2, LOW);
  digitalWrite(dir_2, LOW);
  digitalWrite(MS1_2, LOW);
  digitalWrite(MS2_2, LOW);
  digitalWrite(EN_2, HIGH);
}

void StepForward()
{
  digitalWrite(dir, HIGH); //Pull direction pin low to move "forward"
  digitalWrite(stp,HIGH); //Trigger one step forward
  delay(1);
  digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
  //delay(1);
}
void StepBackward()
{
    digitalWrite(dir, LOW); //Pull direction pin low to move "backward"
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    //delay(1);
}

void WindTheCoil()
{
  digitalWrite(dir_2, HIGH); //Pull direction pin low to move "forward"
    digitalWrite(stp_2,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp_2,LOW); //Pull step pin low so it can be triggered again
    //delay(1);
}

bool isEncoderPositionChanged(){
  encoder.tick();
  int newPos = encoder.getPosition() * (-1);
  if (encoderPosition != newPos) {
    Serial.print("pos:");
    Serial.print(newPos);
    Serial.print(" dir:");
    Serial.println((int)(encoder.getDirection()));
    encoderPosition = newPos;
    return true;
  }
  return false;
}
