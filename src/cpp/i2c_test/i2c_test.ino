#include <Wire.h>

#define I2C_SLAVE_ADDRESS 11 // 12 pour l'esclave 2 et ainsi de suite
#define PAYLOAD_SIZE 2

#define COUNTDOWN_MILLIS 1000

unsigned long prevMillis;
int countdownMillis;

void setup() {
  Wire.begin(I2C_SLAVE_ADDRESS);
  Serial.begin(115200);
  Serial.println("-------------------------------------I am Slave1");
  delay(1000);
  Serial.println("Varoom!");
  prevMillis = millis();
  countdownMillis = COUNTDOWN_MILLIS;
  Wire.onRequest(requestEvents);
  Wire.onReceive(receiveEvents);
}


void loop() {
  unsigned long currentMillis = millis();
  int deltaMillis = currentMillis - prevMillis;
  prevMillis = currentMillis;
  countdownMillis -= deltaMillis;
  if (countdownMillis < 0) {
    Serial.print("Heartbeat ");
    Serial.println(currentMillis);
    countdownMillis = COUNTDOWN_MILLIS;
  }
}

int n = 0;

void requestEvents() {
  Serial.println(F("---> recieved request"));
  Serial.print(F("sending value : "));
  Serial.println(n);
  Wire.write(n);
}

void receiveEvents(int numBytes) {
  Serial.println(F("---> recieved events"));
  Serial.print(numBytes);
  Serial.println(F("bytes recieved"));
  for (int i = 0; i < numBytes; ++i) {
    n = Wire.read();
    Serial.print(F("recieved value : "));
    Serial.println(n);
  }
}