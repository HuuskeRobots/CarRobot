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
#define receiver 8 // pin 1 of IR receiver to Arduino digital pin 8

// Motor 1
#define dir1PinA 7
#define dir2PinA 5
#define speedPinA 6 // Needs to be a PWM pin to be able to control motor speed

// Motor 2
#define dir1PinB 2
#define dir2PinB 4
#define speedPinB 3 // Needs to be a PWM pin to be able to control motor speed

#define ir1Pin 0
#define ir2Pin 1
#define ir3Pin 12
#define ir4Pin 13

#define servoPin 10

#define echoPin 11
#define trigPin 9

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'
Servo servo;  // create servo object to control a servo 
#define middlePos 98
#define maxServoPos middlePos+70
#define minServoPos middlePos-70

#define maxRange 200 // Maximum range needed
#define minRange 0 // Minimum range needed
int val;

int servoPos = middlePos;
int servoPosDelta = 25;
int servoSteps = ((maxServoPos - minServoPos) / servoPosDelta)+1;

#define stateInitial 0
#define stateVooruit 1
#define stateDraaiLinks 2
#define stateDraaiRechts 3
int state = stateInitial;
int richting = 0;
#define maxSpeed 255
/*-----( Declare Variables )-----*/


void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  long d;
  switch (state) {
    case stateInitial:
      servo.write(middlePos);              // tell servo to go to position in variable 'pos' 
      Vooruit();
      state = stateVooruit;
      break;
    case stateVooruit:
      d = measureDistance();
      if (d < 30) {
        Stop();
        if (servoPos > middlePos) {
          state = stateDraaiLinks;
        } else {
          state = stateDraaiRechts;
        }
      }
      break;
     case stateDraaiLinks:
       Linksaf();
       delay(250);
       Stop();
       d = measureDistanceAll();
       if (d > 30) {
         state = stateVooruit;
         Stop();
         Vooruit();
       }
       break;
     case stateDraaiRechts:
       Rechtsaf();
       delay(250);
       Stop();
       d = measureDistanceAll();
       if (d > 30) {
         state = stateVooruit;
         Stop();
         Vooruit();
       }
       break;
  }

/*  if (irrecv.decode(&results)) // have we received an IR signal?

  {
//    Serial.println(results.value, HEX);  UN Comment to see raw values
    translateIR(); 
    irrecv.resume(); // receive the next value
  }*/  
}/* --(end main loop )-- */


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

  // Define ultra sonic pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
 
  // Define servo pin as output 
  servo.attach(servoPin);
}/*--(end setup )---*/

void MotorRechtsVooruit()
{
analogWrite(speedPinA, maxSpeed);//Sets speed variable via PWM 
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
analogWrite(speedPinA, maxSpeed);
digitalWrite(dir1PinA, HIGH);
digitalWrite(dir2PinA, LOW);
Serial.println("Motor 1 Reverse");
}

void MotorLinksVooruit()
{
analogWrite(speedPinB, maxSpeed);
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
analogWrite(speedPinB, maxSpeed);
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

long measureDistanceAll() {
  int i;
  long r = maxRange;
  for (i = 0; i <= servoSteps; i++) {
      r = min(r, measureDistance());
  }
  return r;
}

long measureDistance() {
  servoPos += servoPosDelta;
  if (servoPos > maxServoPos) {
    servoPos = maxServoPos;
    servoPosDelta = -servoPosDelta;
  } else if (servoPos < minServoPos) {
    servoPos = minServoPos;
    servoPosDelta = -servoPosDelta;    
  }
  servo.write(servoPos);
  delay(75);
  //Serial.print("servoPos: ");
  //Serial.println(servoPos);
  
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 25000);
  
  //Calculate the distance (in cm) based on the speed of sound.
  long d = duration/58.2;

  Serial.print("distance: ");
  Serial.println(d);

  if ((d <= minRange) || (d > maxRange)) {
    return maxRange;
  } else {
    return d;
  }
}

void readIR() {
  val = digitalRead(ir1Pin);
  Serial.print("IR1: ");
  Serial.println(val);
  val = digitalRead(ir2Pin);
  Serial.print("IR2: ");
  Serial.println(val);
  val = digitalRead(ir3Pin);
  Serial.print("IR3: ");
  Serial.println(val);
  val = digitalRead(ir4Pin);
  Serial.print("IR4: ");
  Serial.println(val);
}

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

