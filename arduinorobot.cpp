//dattoa deets
//simple udp pc and robot comunication


#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define SDA_PIN A4
#define SCL_PIN A5
#define MTA1 A6
#define MTA2 A7
#define MTB1 D8
#define MTB2 D9
#define ENC_LEFT_A D10
#define ENC_LEFT_B D11
#define ENC_RIGHT_A D12
#define ENC_RIGHT_B D13


const char* ssid = "WIN_304429";
const char* password = "sp6zxbhvqb";

WiFiUDP udp;
const int localPort = 4210;

char incomingPacket[512];

bool hasRun;
bool start;
int state;

int calibrated_left_pwm = 200;
int calibrated_right_pwm = 200;

int calibrateLeft(int speed) {
  return map(speed, 0, 255, 0, 245);
}

int calibrateRight(int speed) {
  return map(speed, 0, 255, 0, 255);
}

void moveForward(int speed, int time) {
  
  calibrated_left_pwm = calibrateLeft(speed);
  calibrated_right_pwm = calibrateRight(speed);



  // move forward
  //A1 A2 control one motor B1 B2 controll other (and need to be flipped to be equivalent)
  //A is right wheel B is left wheel
  analogWrite(MTA1, calibrated_right_pwm);  
  analogWrite(MTB1, LOW);
  analogWrite(MTA2, LOW);  
  analogWrite(MTB2, calibrated_left_pwm);

  delay(time);

  //stop
  analogWrite(MTA1, LOW);  
  analogWrite(MTB1, LOW);
  analogWrite(MTA2, LOW); 
  analogWrite(MTB2, LOW);
}

void executeTask1(){
  if(!hasRun){
    moveForward(250, 5000);
  }
  hasRun = true;
}

void executeTask2(){
  
}

void executeTask3(){
  
}

void executeTask4(){
  
}


void setup() {

  Wire.begin(SDA_PIN, SCL_PIN);


  pinMode(MTA1, OUTPUT);
  pinMode(MTA2, OUTPUT);
  pinMode(MTB1, OUTPUT);
  pinMode(MTB2, OUTPUT);

  start = false;
  state = 0;
  hasRun = false;

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  udp.begin(localPort);
  Serial.printf("UDP server listening on port %d\n", localPort);
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) { //this if loop handles json comunication
    int len = udp.read(incomingPacket, 511);
    if (len > 0) incomingPacket[len] = '\0';

    Serial.println("Received:");
    Serial.println(incomingPacket);

    // Parse JSON
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, incomingPacket);

    if (error) {
      Serial.println("JSON parse failed");
      return;
    }

    //received command
    const char* command = doc["command"];
    const char* msg_id = doc["msg_id"];


    // Prepare response JSON
    StaticJsonDocument<512> response;

    //change state of robot
    if(strcmp(command, "task1") == 0) {
        state = 1;
        response["version"] = "1.0";
        response["msg_id"] = msg_id;
        response["type"] = "response";
        response["status"] = "task1 set";
        response["error"] = nullptr;
    } else if (strcmp(command, "task2") == 0) {
        state = 2;
        response["version"] = "1.0";
        response["msg_id"] = msg_id;
        response["type"] = "response";
        response["status"] = "task2 set";
        response["error"] = nullptr;
    } else if (strcmp(command, "task3") == 0) {
      state = 3;
        response["version"] = "1.0";
        response["msg_id"] = msg_id;
        response["type"] = "response";
        response["status"] = "task3 set";
        response["error"] = nullptr;
    } else if (strcmp(command, "task4") == 0){
        state = 4;
        response["version"] = "1.0";
        response["msg_id"] = msg_id;
        response["type"] = "response";
        response["status"] = "task4 set";
        response["error"] = nullptr;
    } else if (strcmp(command, "start") == 0) {
        start = true;
        response["version"] = "1.0";
        response["msg_id"] = msg_id;
        response["type"] = "response";
        response["status"] = "starting task";
        response["error"] = nullptr;
    } else if (strcmp(command, "stop") == 0) {
        start = false;
        hasRun = false;
        response["version"] = "1.0";
        response["msg_id"] = msg_id;
        response["type"] = "response";
        response["status"] = "stopping task";
        response["error"] = nullptr;
    }


    

    
    //modify json example
    JsonObject data = response.createNestedObject("data");

    data["command"] = command;
    data["start"] = start;
    data["state"] = state;

    // Serialize JSON
    char buffer[512];
    size_t n = serializeJson(response, buffer);

    // Send response
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write((uint8_t*)buffer, n);
    udp.endPacket();

    Serial.println("Response sent.");
  }



  //act on sate / commands

  if(start){
    switch(state){
      case 1:
        executeTask1();
        break;
    }
  }


}