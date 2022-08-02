#include <Wire.h>

#include "black_lights.h"
#include "programs/pulse.h"


#define I2C_SLAVE_ADDRESS 11

Program *program = 0;

unsigned long previousMillis;


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);

  Wire.begin(I2C_SLAVE_ADDRESS);
  Wire.onReceive(receiveEvents);

  Serial.println("-------------------------------------I am Slave1");

  set_all_blacklights(255);

  previousMillis = millis();

  program = new Pulse();
  Pulse *pulse = (Pulse *)program;
  pulse->add_black_light(BLACK_LIGHT_00);
  pulse->add_black_light(BLACK_LIGHT_01);
  pulse->add_black_light(BLACK_LIGHT_02);
  pulse->add_black_light(BLACK_LIGHT_03);
}


void receiveEvents(int numBytes) {
  int offset = Wire.read();
  int bytesSent = Wire.read();

  print("receiveEvent(): offset=");
  print(offset);
  print(" bytesSent=");
  print_ln(bytesSent);

  program->read_data(bytesSent);
}

// void requestByte() {
//   Serial.println("Requesting a byte");
//   int offset = Wire.requestFrom();
//   Serial.println(offset);
//   Serial.println("Done reuesting a byte");
// }

#define COUNTDOWN 2500
int countdown = COUNTDOWN;

void loop() {
  unsigned long currentMillis = millis();  
  unsigned int deltaMillis = currentMillis - previousMillis;
  previousMillis = currentMillis;
  
  countdown -= deltaMillis;
  if (countdown < 0) {
    print_ln(currentMillis);
    countdown = COUNTDOWN;
  }
  
  program->update(deltaMillis);
  program->render();
}

void manualRequest() {
  Serial.println("About to request from i2c");
  Wire.requestFrom(0x30, 2);
  Serial.println("Requested from i2c");
  while (Wire.available()) {
    Serial.println("About to read");
    int c = (int)Wire.read();
    Serial.println("Just read");
    Serial.println(c);
  }
  Serial.println("Done requesting from i2c");
}