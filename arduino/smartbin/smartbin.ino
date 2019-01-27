//www.elegoo.com
//2016.12.9

/* @file smartbin.pde
|| @version 1.0
|| @author Steven Lao
|| @contact lao.a.steven@gmail.com
|| @author Karely Lu
|| @contact mirakl834@gmail.com
||
|| @description
|| | Proof of concept: manually control Arduino to throw an item
|| | in a bin.
|| #
*/

#include <SR04.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <Keypad.h>
#include <Servo.h>

#define ECHO_PIN 6
#define TRIG_PIN 7
#define IR_PIN 12

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);

IRrecv irrecv(IR_PIN);  // create instance of 'irrecv'
decode_results results; // create instance of 'decode_results'

int pos = 0;
long a;
Servo myservo;
Servo servoBottom;

//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};
byte rowPins[ROWS] = {11, 10, 9, 8};
byte colPins[COLS] = {7, 6, 5, 4};

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

/* 
|| Set initial properties.
*/
void setup()
{
  Serial.begin(9600);

  // Set the servos
  servoBottom.attach(2);
  myservo.attach(3);
  servoBottom.write(180);
  myservo.write(90);

  // Infrared
  irrecv.enableIRIn();
}

/* 
|| Normal behavior
*/
void loop()
{

  // Supports Membrane Keypad
  char customKey = customKeypad.getKey();

  if (customKey == 'A')
  {
    myservo.write(30);
    delay(3000);
    myservo.write(90);
  }
  else if (customKey == 'C')
  {
    myservo.write(150);
    delay(3000);
    myservo.write(90);
  }

  // have we received an IR signal?
  if (irrecv.decode(&results))
  {
    switch (results.value)
    {
    case 0xFFA857: // VOL- button pressed
      decrement_slowly(myservo, 90, 65);
      delay(1000);
      decrement_slowly(servoBottom, 180, 70);
      delay(3000);
      increment_slowly(servoBottom, 70, 180);
      increment_slowly(myservo, 65, 90);
      break;

    case 0xFF629D: // VOL+ button pressed
      increment_slowly(myservo, 90, 115);
      delay(1000);
      decrement_slowly(servoBottom, 180, 70);
      delay(3000);
      increment_slowly(servoBottom, 70, 180);
      decrement_slowly(myservo, 115, 90);
      break;
    }
    irrecv.resume(); // receive the next value
  }
  else
  {
    // Ultrasonic sensor
    a = sr04.Distance();
    Serial.print(a);
    Serial.println("cm");
    delay(1000);
  }
}

/* 
|| Move the servomotor at a positive angle, slowly.
*/
void increment_slowly(Servo s, int begin_value, int end_value)
{
  for (pos = begin_value; pos <= end_value; pos += 1)
  {
    s.write(pos);
    delay(15);
  }
}

/* 
|| Move the servomotor at a negative angle, slowly.
*/
void decrement_slowly(Servo s, int begin_value, int end_value)
{
  for (pos = begin_value; pos >= end_value; pos -= 1)
  {
    s.write(pos);
    delay(15);
  }
}
