
#include <SoftwareSerial.h>
//#include <NewSoftSerial.h>
//#define DEBUG

SoftwareSerial ESPport(8, 9); // RX, TX
#define REPEAT_COUNT 10

#define pinSpeed1 2
#define pinSpeed2 3
#define pin1 4
#define pin2 5
#define pin3 6
#define pin4 7
#define trigPin 12
#define echoPin 13
#define SAFE_DISTANCE 10
int val = 0;
String lastCmd;
int repeat = 0;
long gDistance;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  ESPport.begin(9600);
  // put your setup code here, to run once:
  pinMode(pinSpeed1, OUTPUT);
  pinMode(pinSpeed2, OUTPUT);
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  Serial.println("setup...");
  lastCmd = "";
  gDistance = SAFE_DISTANCE;
  delay(1000);
  
}
String getDataFromSerial(String question)
{
    //ESPport.println(question);
    String answer;
    char c;
    int count = 0;
    
    int lastNewLine = 0;
    while (ESPport.available() > 0)
    {
        c = ESPport.read();

        if (lastNewLine == 1)
        {
          answer = "";
        }
        
        if (c == '\n') {
          lastNewLine = 1;
        } else {
          lastNewLine = 0;
          answer += c;
        }
    }
    return answer;
}

void clearSerialPort()
{
#ifdef DEBUG
  Serial.println("clear serial");
#endif
  while  (ESPport.available() > 0) {
    ESPport.read();
  } 
}

long getDistance()
{
  long duration;
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 30000);
  if (duration == 0) {duration = SAFE_DISTANCE;}
  return (duration/2) / 29.1;
}

#define MEASERMENTS_COUNT 3
long getForwardDistance()
{
  long distance = getDistance();

  for (unsigned int i = 0; i < (MEASERMENTS_COUNT-1); ++i)
  {
    distance += getDistance();
  }
  distance = (distance/MEASERMENTS_COUNT);
  Serial.print("getForwardDistance: ");Serial.println(distance);
  return distance;
}
void stopMotors()
{
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
}

void loop()
{
   // read the input on analog pin 0:
  //int sensorValue = analogRead(0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  //float voltage = sensorValue * (5.0 / 1023.0);
  //Serial.print("Voltage: ");Serial.println(voltage);
  

    String cmd = getDataFromSerial("=cmd");
    //Serial.println(cmd);

    if (cmd.length() == 0)
    {
      if (repeat > 0)
      {
        cmd = lastCmd;
        repeat--;
      } else {
        cmd = "none";
      }
    }
    if (cmd.length() >= 2)
    {
      // 0-255
      int speedValue = 100;
      switch (cmd[1])
      {
        case '1': speedValue = 130; break;
        case '2': speedValue = 150; break;
        case '3': speedValue = 155; break;
        case '4': speedValue = 160; break;
        case '5': speedValue = 165; break;
        case '6': speedValue = 170; break;
        case '7': speedValue = 180; break;
        case '8': speedValue = 200; break;
        case '9': speedValue = 240; break;
      }
#ifdef DEBUG
      Serial.print("set speed: '");
      Serial.print(speedValue);
      Serial.println("'");
#endif
      analogWrite(pinSpeed1, speedValue);
      analogWrite(pinSpeed2, speedValue);
    }
    
    //if (gDistance < SAFE_DISTANCE) {stopMotors();}
    //Serial.print("gDistance: ");Serial.println(gDistance);
    
    if (cmd[0] == 'f')
    {
      lastCmd = cmd;
      repeat = REPEAT_COUNT;
      #ifdef DEBUG
      Serial.println("go: '" + cmd + "'");
      #endif

      //long _tmpDistance = getForwardDistance();

      //gDistance = (_tmpDistance+gDistance)/2;
      //if (_tmpDistance < gDistance) {gDistance = _tmpDistance;}

      //if (gDistance >= SAFE_DISTANCE)
      //{
        digitalWrite(pin1, LOW);
        digitalWrite(pin2, HIGH);
        digitalWrite(pin3, HIGH);
        digitalWrite(pin4, LOW);
      //}
      //else {stopMotors();}
      
    }
    else if (cmd[0] == 'l')
    {
      lastCmd = cmd;
      repeat = REPEAT_COUNT;
      #ifdef DEBUG
      Serial.println("left: '" + cmd + "'");
      #endif
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, HIGH);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, HIGH);
      //gDistance = SAFE_DISTANCE;
    }
    else if (cmd[0] == 'r')
    {
      lastCmd = cmd;
      repeat = REPEAT_COUNT;
      #ifdef DEBUG
      Serial.println("right: '" + cmd + "'");
      #endif
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, HIGH);
      digitalWrite(pin4, LOW);
      //gDistance = SAFE_DISTANCE;
    }
    else if (cmd[0] == 'b')
    {
      lastCmd = cmd;
      repeat = REPEAT_COUNT;
      #ifdef DEBUG
      Serial.println("back: '" + cmd + "'");
      #endif
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, HIGH);

      //long _tmpDistance = getForwardDistance();
      //if (_tmpDistance > gDistance) {gDistance = _tmpDistance;}
    }
    else
    {
      lastCmd = cmd;
      repeat = REPEAT_COUNT;
      #ifdef DEBUG
      Serial.println("stop: '" + cmd + "'");
      #endif
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, LOW);
      
    }
  delay(5);
}
