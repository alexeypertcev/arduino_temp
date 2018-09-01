
#include <SoftwareSerial.h>
//#include <NewSoftSerial.h>

SoftwareSerial mySerial(9, 8); // RX, TX


int pin1 = 2;
int pin2 = 3;
int pin3 = 4;
int pin4 = 5;

int val = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  mySerial.begin(9600);
  // put your setup code here, to run once:
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  
  Serial.println("setup...");
  delay(1000);
  
}
String getDataFromSerial(String question)
{
    delay(10);
    mySerial.println(question);
    delay(10);
    String answer;
    char c = 32;
    //int count = 20;

    if  (mySerial.available() > 0) {
      while (c != '\n') {
        c = mySerial.read();
      } 
      
      do {
          c = mySerial.read();
          if (c == '\n') {break;}
          answer+=c;
      } while (1);

    }
    return answer;
}

void clearSerialPort()
{
  Serial.println("clear serial");
  delay(10);
  while  (mySerial.available() > 0) {
    mySerial.read();
  } 
  delay(10);
}

void loop() {
  // put your main code here, to run repeatedly:
//wer supply for the motors and another pow
  //int buttonState = digitalRead(pinB);
  //Serial.print("buttonState: ");
  //Serial.println(buttonState);
  
  //if (buttonState)
  {
    String pos = getDataFromSerial("=cmd.pos");
    String res = "cmd.pos = " + pos;
    Serial.println(pos);

    if (pos[0] == 's' || pos[0] == '0')
    {
      Serial.println("stop");
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, LOW);
    }
    else if (pos[0] == 'f')
    {
      Serial.println("go");
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, HIGH);
      digitalWrite(pin3, HIGH);
      digitalWrite(pin4, LOW);
    }
    else if (pos[0] == 'l')
    {
      Serial.println("left");
      digitalWrite(pin1, LOW);
      digitalWrite(pin2, HIGH);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, HIGH);
    }
    else if (pos[0] == 'r')
    {
      Serial.println("right");
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, HIGH);
      digitalWrite(pin4, LOW);
    }
    else if (pos[0] == 'b')
    {
      Serial.println("back");
      digitalWrite(pin1, HIGH);
      digitalWrite(pin2, LOW);
      digitalWrite(pin3, LOW);
      digitalWrite(pin4, HIGH);
    }
    else
    {
      clearSerialPort();
    }
  }
  delay(330);
}
