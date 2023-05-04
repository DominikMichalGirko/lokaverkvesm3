#include "PubSubClient.h" // Connect and publish to the MQTT broker

// Code for the ESP32
#include "WiFi.h" // Enables the ESP32 to connect to the local network (via WiFi)

// WiFi
const char* ssid = "TskoliVESM";                 // Your personal network SSID
const char* wifi_password = "Fallegurhestur";        // Your personal network password

// MQTT
const char* mqtt_server = "10.201.48.65";  // IP of the MQTT broker
const char* mqtt_username = "dommi"; // MQTT username
const char* mqtt_password = "12345"; // MQTT password
const char* Light_topic = "home/VesmLight";
const char* clientID = "client_GreenRedVesm"; // MQTT client ID
const char* GameOn = "home/GameOnVesm";


// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
// 1883 is the listener port for the Broker
PubSubClient client(mqtt_server, 1883, wifiClient); 


// Custom function to connet to the MQTT broker via WiFi
void connect_MQTT(){
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connect to the WiFi
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Debugging - Output the IP Address of the ESP8266
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to MQTT Broker
  // client.connect returns a boolean value to let us know if the connection was successful.
  // If the connection is failing, make sure you are using the correct MQTT Username and Password (Setup Earlier in the Instructable)
  if (client.connect(clientID,mqtt_username, mqtt_password)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }
}


int Trig = 5;    // TRIG pin
int Echo = 18;    // ECHO pin

int red = 17;
int green = 16;

int buzzer = 13;

int RedLight = 0, ehvhreyfing = 0, GreenLight = 0;

// put your main code here, to run repeatedly:
  void callback(char* Topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(Topic);
    Serial.print(". Message: ");
    String messageTemp;

     for (int i = 0; i < length; i++) {
      Serial.print((char)message[i]);
      messageTemp += (char)message[i];
  }
  Serial.println();

   if(messageTemp == "Green"){
     Serial.println("GreenLight");
     digitalWrite(green, HIGH);
     digitalWrite(red,LOW);
    }
    else if(messageTemp == "Red"){
      Serial.println("redLight");
      digitalWrite(red, HIGH);
      digitalWrite(green,LOW);
    }
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
  Serial.print("grænt ljós u can go");       
  return (int)tester;
}  

void setup() {
  // begin serial port
  connect_MQTT();
  Serial.begin (9600);

  // configure the trigger pin to output mode
  pinMode(Trig, OUTPUT);
  // configure the echo pin to input mode
  pinMode(Echo, INPUT);
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(buzzer,OUTPUT);
  client.setCallback(callback);
}

void loop() {
  digitalWrite(buzzer,LOW);
  // PUBLISH to the MQTT Broker (topic = Temperature, defined at the beginning)
  // .c_str() converts a string to an array of characters and terminates this array with a null character at the end.
  client.subscribe(GameOn);
    if(red == HIGH){
      RedLight = Distance_test();
      delay(200);
      ehvhreyfing = hreyfing();
      if(ehvhreyfing < RedLight){
        digitalWrite(buzzer,HIGH);
        String hre = "hreyfing";
        if (client.publish(Light_topic, String(hre).c_str())) { 
          Serial.println("hreyfing sent");
        }
        // Again, client.publish will return a boolean value depending on whether it succeded or not.
        // If the message failed to send, we will try again, as the connection may have broken.
        else {
        Serial.println("failed");
        client.connect(clientID);
        delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
        client.publish(Light_topic, String(hre).c_str());
        }
      }
    }
      delay(3000);
      if(green == HIGH){
        GreenLight = eyesclosed();
        digitalWrite(buzzer,LOW);
      }
  delay(3000);
}