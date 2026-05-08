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
#define LINE_SENSOR A1



const char* ssid = "WIN_304429";
const char* password = "sp6zxbhvqb";

WiFiUDP udp;
const int localPort = 4210;

char incomingPacket[512];

bool hasRun;
bool start;
int state;
int threshold;

int calibrated_left_pwm = 200;
int calibrated_right_pwm = 200;

void sendPosUpdate(int dx, int dy, int id){
  // Prepare response JSON
    StaticJsonDocument<512> update;

    update["version"] = "1.0";
    update["msg_id"] = id;
    update["type"] = "update";
    update["status"] = "pos update";
    update["error"] = nullptr;

    JsonObject data = update.createNestedObject("data");

    data["dx"] = dx;
    data["dy"] = dy;

    // Serialize JSON
    char buffer[512];
    size_t n = serializeJson(update, buffer);

    // Send response
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write((uint8_t*)buffer, n);
    udp.endPacket();
}

int calibrateLeft(int speed) {
  return map(speed, 0, 255, 0, 220);
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

void turnRight(int speed, int time){
  calibrated_left_pwm = calibrateLeft(speed);
  calibrated_right_pwm = calibrateRight(speed);



  // move forward
  //A1 A2 control one motor B1 B2 controll other (and need to be flipped to be equivalent)
  //A is right wheel B is left wheel
  analogWrite(MTA1, LOW);  
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

void turnLeft(int speed, int time){
  calibrated_left_pwm = calibrateLeft(speed);
  calibrated_right_pwm = calibrateRight(speed);



  // move forward
  //A1 A2 control one motor B1 B2 controll other (and need to be flipped to be equivalent)
  //A is right wheel B is left wheel
  analogWrite(MTA1, calibrated_right_pwm);  
  analogWrite(MTB1, LOW);
  analogWrite(MTA2, LOW);  
  analogWrite(MTB2, LOW);

  delay(time);

  //stop
  analogWrite(MTA1, LOW);  
  analogWrite(MTB1, LOW);
  analogWrite(MTA2, LOW); 
  analogWrite(MTB2, LOW);
}


void right90(){
  turnRight(100, 1070);
}

void left90(){
  turnLeft(100, 1070);
}


void executeTask1(){
  if(!hasRun){
    moveForward(250, 7500);
    sendPosUpdate(1,1,1);
    sendPosUpdate(1,1,2);
    sendPosUpdate(1,1,3);
    sendPosUpdate(1,1,4);
    sendPosUpdate(1,1,5);
    sendPosUpdate(1,1,6);
    sendPosUpdate(1,1,7);
    sendPosUpdate(1,1,8);
  }
  hasRun = true;
}

void executeTask2(){
  if(!hasRun){
    // right90();
    // delay(2000);
    // left90();

    
    moveForward(250, 4000);
    sendPosUpdate(1,0,1);
    sendPosUpdate(1,0,2);
    sendPosUpdate(1,0,3);
    sendPosUpdate(1,0,4);
    sendPosUpdate(1,0,5);
    sendPosUpdate(1,0,6);
    delay(2000);
    right90();
    delay(2000);
    moveForward(250, 4000);
    sendPosUpdate(0,1,7);
    sendPosUpdate(0,1,8);
    sendPosUpdate(0,1,9);
    sendPosUpdate(0,1,10);
    sendPosUpdate(0,1,11);
    sendPosUpdate(0,1,12);
    delay(2000);
    right90();
    delay(2000);
    moveForward(250, 4000);
    sendPosUpdate(-1,0,13);
    sendPosUpdate(-1,0,14);
    sendPosUpdate(-1,0,15);
    sendPosUpdate(-1,0,16);
    sendPosUpdate(-1,0,17);
    sendPosUpdate(-1,0,18);
    delay(2000);
    right90();
    delay(2000);
    moveForward(250, 4000);
    sendPosUpdate(0,-1,19);
    sendPosUpdate(0,-1,20);
    sendPosUpdate(0,-1,21);
    sendPosUpdate(0,-1,22);
    sendPosUpdate(0,-1,23);
    sendPosUpdate(0,-1,24);
    
  }
  hasRun = true;
}

void executeTask3(){
  if(!hasRun){
    // right90();
    // delay(2000);
    // left90();

    
    moveForward(250, 4000);
    sendPosUpdate(1,0,1);
    sendPosUpdate(1,0,2);
    sendPosUpdate(1,0,3);
    sendPosUpdate(1,0,4);
    sendPosUpdate(1,0,5);
    sendPosUpdate(1,0,6);
    delay(2000);
    right90();
    delay(2000);
    moveForward(250, 4000);
    sendPosUpdate(0,1,7);
    sendPosUpdate(0,1,8);
    sendPosUpdate(0,1,9);
    sendPosUpdate(0,1,10);
    sendPosUpdate(0,1,11);
    sendPosUpdate(0,1,12);
    delay(2000);
    right90();
    delay(2000);
    moveForward(250, 4000);
    sendPosUpdate(-1,0,13);
    sendPosUpdate(-1,0,14);
    sendPosUpdate(-1,0,15);
    sendPosUpdate(-1,0,16);
    sendPosUpdate(-1,0,17);
    sendPosUpdate(-1,0,18);
    delay(2000);
    right90();
    delay(2000);
    moveForward(250, 4000);
    sendPosUpdate(0,-1,19);
    sendPosUpdate(0,-1,20);
    sendPosUpdate(0,-1,21);
    sendPosUpdate(0,-1,22);
    sendPosUpdate(0,-1,23);
    sendPosUpdate(0,-1,24);

    delay(2000);
    left90();
    delay(1000);
    left90();
    delay(2000);

    moveForward(250, 4000);
    sendPosUpdate(0,1,25);
    sendPosUpdate(0,1,26);
    sendPosUpdate(0,1,27);
    sendPosUpdate(0,1,28);
    sendPosUpdate(0,1,29);
    sendPosUpdate(0,1,30);
    delay(2000);
    left90();
    delay(2000);
    moveForward(250, 4000);
    sendPosUpdate(1,0,31);
    sendPosUpdate(1,0,32);
    sendPosUpdate(1,0,33);
    sendPosUpdate(1,0,34);
    sendPosUpdate(1,0,35);
    sendPosUpdate(1,0,36);
    delay(2000);
    left90();
    delay(2000);
    moveForward(250, 4000);
    sendPosUpdate(0,-1,37);
    sendPosUpdate(0,-1,38);
    sendPosUpdate(0,-1,39);
    sendPosUpdate(0,-1,40);
    sendPosUpdate(0,-1,41);
    sendPosUpdate(0,-1,42);
    delay(2000);
    left90();
    delay(2000);
    moveForward(250, 4000);
    sendPosUpdate(-1,0,43);
    sendPosUpdate(-1,0,44);
    sendPosUpdate(-1,0,45);
    sendPosUpdate(-1,0,46);
    sendPosUpdate(-1,0,47);
    sendPosUpdate(-1,0,48);

  }
  hasRun = true;
}

void executeTask4(){ //follow line via infared
  if(!hasRun){
    int lapTime = 4350;
    for(int i = 0; i < lapTime; i++){ 
      int seeLine = analogRead(LINE_SENSOR);
      if (seeLine < threshold) { //line detected
        analogWrite(MTA1, LOW);  //turn left
        analogWrite(MTB1, LOW);
        analogWrite(MTA2, 100);  
        analogWrite(MTB2, LOW);
      } else { //no line
        analogWrite(MTA1, LOW);  
        analogWrite(MTB1, 100);
        analogWrite(MTA2, LOW);  
        analogWrite(MTB2, LOW);
      }
      delay(10);
    }
    analogWrite(MTA1, LOW);  
    analogWrite(MTB1, LOW);
    analogWrite(MTA2, LOW);  
    analogWrite(MTB2, LOW);
  }
  hasRun = true;
}

void executeTask5(){
  if(!hasRun){
    int lapTime = 4350;
    for(int i = 0; i < lapTime; i++){ 
      int seeLine = analogRead(LINE_SENSOR);
      if (seeLine < threshold) { //line detected
        analogWrite(MTA1, LOW);  //turn left
        analogWrite(MTB1, LOW);
        analogWrite(MTA2, 100);  
        analogWrite(MTB2, LOW);
      } else { //no line
        analogWrite(MTA1, LOW);  
        analogWrite(MTB1, 100);
        analogWrite(MTA2, LOW);  
        analogWrite(MTB2, LOW);
      }
      if(i < 4400 && i >= 3300 && i%100 == 0){
        sendPosUpdate(0,-1,i);
      }else if(i < 3300 && i >= 2200 && i%100 == 0){
        sendPosUpdate(-1,0,i);
      }else if(i < 2200 && i >= 1100 && i%100 == 0){
        sendPosUpdate(0,1,i);
      }else if(i < 1100 && i%100 == 0){
        sendPosUpdate(1,0,i);
      }
      delay(10);
    }
    analogWrite(MTA1, LOW);  
    analogWrite(MTB1, LOW);
    analogWrite(MTA2, LOW);  
    analogWrite(MTB2, LOW);
  }
  hasRun = true;
}

void executeTask6(){ //draw a letter
  
}

void testIRSensor(){
  while(true){
    
    Serial.print("a0: ");
    Serial.println(analogRead(A0));
    Serial.print(" ");
    Serial.print("a1: ");
    Serial.println(analogRead(A1));
    Serial.print(" ");
    Serial.print("a2: ");
    Serial.println(analogRead(A2));
    Serial.print(" ");
    Serial.print("a3: ");
    Serial.println(analogRead(A3));

    Serial.println();

    delay(200);
  }
}

void setup() {

  Wire.begin(SDA_PIN, SCL_PIN);


  pinMode(MTA1, OUTPUT);
  pinMode(MTA2, OUTPUT);
  pinMode(MTB1, OUTPUT);
  pinMode(MTB2, OUTPUT);
  pinMode(LINE_SENSOR, INPUT); //why is this not a change

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);




  start = false;
  state = 0;
  hasRun = false;
  threshold = 4000;

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
    } else if (strcmp(command, "task5") == 0){
        state = 5;
        response["version"] = "1.0";
        response["msg_id"] = msg_id;
        response["type"] = "response";
        response["status"] = "task5 set";
        response["error"] = nullptr;
    } else if (strcmp(command, "task6") == 0){
        state = 6;
        response["version"] = "1.0";
        response["msg_id"] = msg_id;
        response["type"] = "response";
        response["status"] = "task6 set";
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
      case 2:
        executeTask2();
        break;
      case 3:
        executeTask3();
        break;
      case 4:
        executeTask4();
        break;
      case 5:
        executeTask5();
        break;
      case 6:
        executeTask6();
        break;
      case 10:
        testIRSensor();
        break;
    }
  }


}