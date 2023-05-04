int Trig = 5;    // TRIG pin
int Echo = 18;    // ECHO pin

int red = 16;
int green = 17;

int buzzer = 13;

int RedLight = 0, ehvhreyfing = 0, GreenLight = 0;

#include "HardwareSerial.h"
#include "DFRobotDFPlayerMini.h"

const byte RXD2 = 16; // Connects to module's RX 
const byte TXD2 = 17; // Connects to module's TX 

HardwareSerial dfSD(1); // Use UART channel 1
DFRobotDFPlayerMini player;

void hljod(){
  digitalWrite(buzzer, HIGH);
}

int Distance_test() {
  digitalWrite(Trig, LOW);   
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);   
  float Fdistance = pulseIn(Echo, HIGH);  
  Fdistance= Fdistance / 58;       
  return (int)Fdistance;
}  


int hreyfing() {
  digitalWrite(Trig, LOW);    
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);   
  float tester = pulseIn(Echo, HIGH);  
  tester= tester / 58;       
  return (int)tester;
}  

int eyesclosed() {
  digitalWrite(Trig, LOW);   
  delayMicroseconds(2);
  digitalWrite(Trig, LOW);  
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);   
  float tester = pulseIn(Echo, LOW);  
  tester= tester / 58;
  Serial.print("grænt ljós u can go:");       
  return (int)tester;
}  

void setup() {
  // begin serial port
  Serial.begin (9600);


  {
  Serial.begin(19200);
  dfSD.begin(9600, SERIAL_8N1, RXD2, TXD2);  // 16,17
  delay(5000);

  if (player.begin(dfSD)) 
  {
    Serial.println("OK");

    // Set volume to maximum (0 to 30).
    player.volume(17); //30 is very loud
  } 
  else 
  {
    Serial.println("Connecting to DFPlayer Mini failed!");
  }
}
  // configure the trigger pin to output mode
  pinMode(Trig, OUTPUT);
  // configure the echo pin to input mode
  pinMode(Echo, INPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(buzzer,OUTPUT);
}

void loop() {
     RedLight = Distance_test();
     delay(200);
      ehvhreyfing = hreyfing();
      digitalWrite(buzzer,LOW);
      Serial.println("Playing #1");
  player.play(1);
  Serial.println("play start");
  delay(5000);
  Serial.println("played:");
  delay(1000);

  Serial.println("Playing #2");
  player.play(2);
  Serial.println("play start");
  delay(10000);
  Serial.println("played:");
  delay(1000);

  Serial.println("Playing #3");
  player.play(3);
  Serial.println("play start");
  delay(10000);
  Serial.println("played:");
  delay(1000);

  Serial.println("Playing #4");
  player.play(4);
  Serial.println("play start");
  delay(10000);
  Serial.println("played\r\n\r\n");
  delay(1000);
      digitalWrite(green,LOW);
      
      digitalWrite(red,HIGH);
      if(ehvhreyfing < RedLight){
        Serial.print("Hreyfing á meðan Red Light:");
        hljod();
      }
      delay(3000);
      digitalWrite(red,LOW);
      digitalWrite(buzzer,LOW);
      //mp3 player kóði hér  Áfram!
      GreenLight = eyesclosed();
      digitalWrite(green,HIGH);
    
  delay(3000);
}