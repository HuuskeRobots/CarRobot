/* YourDuino.com Example Software Sketch
 IR Remote Kit Test
 Uses YourDuino.com IR Infrared Remote Control Kit 2
 http://arduino-direct.com/sunshop/index.php?l=product_detail&p=153
 based on code by Ken Shirriff - http://arcfn.com
 Get Library at: https://github.com/shirriff/Arduino-IRremote
 Unzip folder into Libraries. RENAME folder IRremote
 terry@yourduino.com */

/*-----( Import needed libraries )-----*/

#include <IRremote.h>
#include <IRremoteInt.h>
#include <Servo.h> 

/*-----( Declare Constants )-----*/
int receiver = 8; // pin 1 of IR receiver to Arduino digital pin 8

// Motor 1
int dir1PinA = 7;
int dir2PinA = 5;
int speedPinA = 6; // Needs to be a PWM pin to be able to control motor speed

// Motor 2
int dir1PinB = 2;
int dir2PinB = 4;
int speedPinB = 3; // Needs to be a PWM pin to be able to control motor speed

int ir1Pin = 0;
int ir2Pin = 1;
int ir3Pin = 12;
int ir4Pin = 13;

int servoPin = 10;

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'
Servo servo;  // create servo object to control a servo 
int servoPos = 0;
int servoDelta = 5;
/*-----( Declare Variables )-----*/



void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Serial.begin(9600);
  Serial.println("IR Receiver Raw Data + Button Decode Test");
  irrecv.enableIRIn(); // Start the receiver

  //Define L298N Dual H-Bridge Motor Controller Pins
  pinMode(dir1PinA,OUTPUT);
  pinMode(dir2PinA,OUTPUT);
  pinMode(speedPinA,OUTPUT);
  pinMode(dir1PinB,OUTPUT);
  pinMode(dir2PinB,OUTPUT);
  pinMode(speedPinB,OUTPUT);
  
  // Define all IR's as input 
  pinMode(ir1Pin, INPUT);
  pinMode(ir2Pin, INPUT);
  pinMode(ir3Pin, INPUT);
  pinMode(ir4Pin, INPUT);
  
  // Define servo pin as output 
  servo.attach(servoPin);
}/*--(end setup )---*/


void MotorRechtsVooruit()
{
analogWrite(speedPinA, 255);//Sets speed variable via PWM 
digitalWrite(dir1PinA, LOW);
digitalWrite(dir2PinA, HIGH);
Serial.println("Motor 1 Forward"); // Prints out “Motor 1 Forward” on the serial monitor
}

void MotorRechtsStop()
{
analogWrite(speedPinA, 0);
digitalWrite(dir1PinA, LOW);
digitalWrite(dir2PinA, LOW);
Serial.println("Motor 1 Stop");
}

void MotorRechtsAchteruit()
{
analogWrite(speedPinA, 255);
digitalWrite(dir1PinA, HIGH);
digitalWrite(dir2PinA, LOW);
Serial.println("Motor 1 Reverse");
}

void MotorLinksVooruit()
{
analogWrite(speedPinB, 255);
digitalWrite(dir1PinB, LOW);
digitalWrite(dir2PinB, HIGH);
Serial.println("Motor 2 Forward");
}

void MotorLinksStop()
{
analogWrite(speedPinB, 0);
digitalWrite(dir1PinB, LOW);
digitalWrite(dir2PinB, LOW);
Serial.println("Motor 2 Stop");
}

void MotorLinksAchteruit()
{
analogWrite(speedPinB, 255);
digitalWrite(dir1PinB, HIGH);
digitalWrite(dir2PinB, LOW);
Serial.println("Motor 2 Reverse");
}

void Rechtsaf()
{
  MotorLinksVooruit();
  MotorRechtsAchteruit();
}

void Linksaf()
{
  MotorRechtsVooruit();
  MotorLinksAchteruit();
}

void Vooruit()
{
  MotorLinksVooruit();
  MotorRechtsVooruit();
}

void Achteruit()
{
  MotorLinksAchteruit();
  MotorRechtsAchteruit();
}

void Stop()
{
  MotorRechtsStop();
  MotorLinksStop();
}


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
    servo.write(servoPos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
    servoPos = servoPos + servoDelta;
    if (servoPos > 180) {
      servoPos = 180;
      servoDelta = -servoDelta;
    } else if (servoPos < 0) {
      servoPos = 0;
      servoDelta = -servoDelta;
    }

  if (irrecv.decode(&results)) // have we received an IR signal?

  {
//    Serial.println(results.value, HEX);  UN Comment to see raw values
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
}/* --(end main loop )-- */

/*-----( Declare User-written Functions )-----*/
void translateIR() // takes action based on IR code received

// describing Car MP3 IR codes 

{

  switch(results.value)

  {

  case 0xFFA25D:  
    Serial.println(" CH-            "); 
    break;

  case 0xFF629D:  
    Serial.println(" CH             "); 
    break;

  case 0xFFE21D:  
    Serial.println(" CH+            "); 
    break;

  case 0xFF22DD:  
    Serial.println(" PREV           "); 
    // Links
    Linksaf();
    break;

  case 0xFF02FD:  
    Serial.println(" NEXT           "); 
    //Rechts
    Rechtsaf();
    break;

  case 0xFFC23D:  
    Serial.println(" PLAY/PAUSE     "); 
    // Stop
    Stop();
    break;

  case 0xFFE01F:  
    Serial.println(" VOL-           "); 
    // Achteruit
    Achteruit();
    break;

  case 0xFFA857:  
    Serial.println(" VOL+           "); 
    // Vooruit
    Vooruit();
    break;

  case 0xFF906F:  
    Serial.println(" EQ             "); 
    break;

  case 0xFF6897:  
    Serial.println(" 0              "); 
    break;

  case 0xFF9867:  
    Serial.println(" 100+           "); 
    break;

  case 0xFFB04F:  
    Serial.println(" 200+           "); 
    break;

  case 0xFF30CF:  
    Serial.println(" 1              "); 
    break;

  case 0xFF18E7:  
    Serial.println(" 2              "); 
    break;

  case 0xFF7A85:  
    Serial.println(" 3              "); 
    break;

  case 0xFF10EF:  
    Serial.println(" 4              "); 
    break;

  case 0xFF38C7:  
    Serial.println(" 5              "); 
    break;

  case 0xFF5AA5:  
    Serial.println(" 6              "); 
    break;

  case 0xFF42BD:  
    Serial.println(" 7              "); 
    break;

  case 0xFF4AB5:  
    Serial.println(" 8              "); 
    break;

  case 0xFF52AD:  
    Serial.println(" 9              "); 
    break;

  default: 
    Serial.println(" other button   ");

  }

  delay(500);


} //END translateIR



/* ( THE END ) */

