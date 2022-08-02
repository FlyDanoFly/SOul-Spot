#include <Wire.h>

typedef enum {
  STATE_INIT,
  STATE_ASCENDING,
  STATE_HOLD_HIGH,
  STATE_DESCENDING,
  STATE_HOLD_LOW,
} STATE;

typedef struct {
  /* data */
  unsigned int lowIntensity;
  unsigned int highIntensity;
  unsigned int ascendingMillis;
  unsigned int holdHighMillis;
  unsigned int descendingMillis;
  unsigned int holdLowMillis;
} STRUCT_PROGRAM;

int i = 0;
unsigned long previousMillis = 0;
unsigned long nextMillis = 0;
int ledState = LOW;
int intensity;
unsigned long lastLogMillis = 0;

STATE currentState = STATE_INIT;
STRUCT_PROGRAM params = {5, 220, 2000, 4000, 2000, 1000};
#define I2C_SLAVE_ADDRESS 11

// the setup function runs once when you press reset or power the board
void setup() {
  Wire.begin(I2C_SLAVE_ADDRESS);
  Wire.onReceive(receiveEvents);

  Serial.println("-------------------------------------I am Slave1");
//  Wire.begin(0x20);
//  Wire.onReceive(receiveEvents);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN1, OUTPUT);
  pinMode(PIN2, OUTPUT);
  pinMode(PIN3, OUTPUT);
  pinMode(PIN5, OUTPUT);

  for (int pin = 0; pin <= 13; pin++) {
    pinMode(pin, OUTPUT);
  }
  currentState = STATE_ASCENDING;
  previousMillis = millis();
  nextMillis = previousMillis + params.ascendingMillis;
  intensity = params.lowIntensity;

  Serial.begin(115200);
  lastLogMillis = millis();
  Serial.println("Starting output...");
  Serial.println(params.lowIntensity);
  Serial.println(params.highIntensity);
  Serial.println(params.ascendingMillis);
  Serial.println(params.holdHighMillis);
  Serial.println(params.descendingMillis);
  Serial.println(params.holdLowMillis);
}

int n = 0;
int alt_n = -1;

void receiveEvents(int numBytes) {
  Serial.println(F("---> recieved events"));
  Serial.println(millis());
  int offset = Wire.read();
  Serial.print(numBytes);
  Serial.println(F("bytes received"));
  Serial.print(F("received offset: "));
  Serial.println(offset);
  Serial.print(F("received value: "));
  alt_n = Wire.read();
  Serial.println(alt_n);
  Serial.println("done with delay");
  Serial.println(millis());
}

// void requestByte() {
//   Serial.println("Requesting a byte");
//   int offset = Wire.requestFrom();
//   Serial.println(offset);
//   Serial.println("Done reuesting a byte");
// }

void set_pins(int intensity) {
  Serial.println(intensity);
  for (int pin = 0; pin <=13; pin++) {
    analogWrite(pin, intensity);
  }
}

void loop() {
  Serial.println("On");
  int squid = 0;

  squid = 255;
  set_pins(squid);
  delay(1000);
  squid = 55;
  set_pins(squid);
  delay(1000);
  squid = 155;
  set_pins(squid);
  delay(1000);
  squid = 100;
  set_pins(squid);
  delay(1000);
  squid = 0;
  set_pins(squid);
  delay(1000);
}

// the loop function runs over and over again forever
void loop2() {
  unsigned long currentMillis = millis();

  // Serial.println('Writing 42');
  // Wire.beginTransmission();
  // Wire.write(42);
  // Wire.endTransmission();
  // Serial.println('Writing 42 done');
  if ((0 <= alt_n) && (alt_n < 255)) {
    Serial.println('oogabooga');
    intensity = alt_n;
    intensity = constrain(intensity, params.lowIntensity, params.highIntensity);
    analogWrite(PIN2, intensity);
  } else
  if (currentState == STATE_ASCENDING) {
    unsigned long diff = currentMillis - previousMillis;
    intensity = params.lowIntensity +  (params.highIntensity - params.lowIntensity) * diff / (nextMillis - previousMillis);

    // Change state?
    if (currentMillis > nextMillis) {
      Serial.println("Change state to STATE_HOLD_HIGH");
      previousMillis = currentMillis;
      nextMillis = currentMillis + params.holdHighMillis;
      currentState = STATE_HOLD_HIGH;
      intensity = params.highIntensity;
      ledState = (ledState == LOW) ? HIGH : LOW;
      digitalWrite(LED_BUILTIN, ledState);
    }

    intensity = constrain(intensity, params.lowIntensity, params.highIntensity);
    Serial.print("Writing intensity: ");
    Serial.println(intensity);
    analogWrite(PIN2, intensity);
  } else if (currentState == STATE_HOLD_HIGH) {
    // do nothing in hold high, just hold
    intensity = params.highIntensity;
    
    // Change state?
    if (currentMillis > nextMillis) {
      Serial.println("Change state to STATE_DESCENDING");
      previousMillis = currentMillis;
      nextMillis = currentMillis + params.descendingMillis;
      currentState = STATE_DESCENDING;
      ledState = (ledState == LOW) ? HIGH : LOW;
      digitalWrite(LED_BUILTIN, ledState);
    }
  } else if (currentState == STATE_DESCENDING) {
    unsigned long diff = currentMillis - previousMillis;
    intensity = params.highIntensity - (params.highIntensity - params.lowIntensity) * diff / (nextMillis - previousMillis);

    // Change state?
    if (currentMillis > nextMillis) {
      Serial.println("Change state to STATE_HOLD_LOW");
      previousMillis = currentMillis;
      nextMillis = currentMillis + params.holdLowMillis;
      Serial.println("Hold low millis");
      Serial.println(params.holdLowMillis);
      currentState = STATE_HOLD_LOW;
      intensity = params.lowIntensity;
      ledState = (ledState == LOW) ? HIGH : LOW;
      digitalWrite(LED_BUILTIN, ledState);
    }

    intensity = constrain(intensity, params.lowIntensity, params.highIntensity);
    analogWrite(PIN2, intensity);
  } else if (currentState == STATE_HOLD_LOW) {
    // do nothing in hold low, just hold
    intensity = params.lowIntensity;

    // Change state?
    if (currentMillis > nextMillis) {
      Serial.println("Change state to STATE_ASCENDING");
      previousMillis = currentMillis;
      nextMillis = currentMillis + params.ascendingMillis;
      currentState = STATE_ASCENDING;
      ledState = (ledState == LOW) ? HIGH : LOW;
      digitalWrite(LED_BUILTIN, ledState);
    }
  }

  // maunalRequest();

  if (true && (currentMillis > lastLogMillis + 250)) {
    Serial.println("Heatbeat: state, intensity, time to next (secs), time to next (millis), current millis");
    Serial.print(currentState);
    Serial.print(", ");
    Serial.print(intensity);
    Serial.print(", ");
    Serial.print((nextMillis - currentMillis) / 1000);
    Serial.print(", ");
    Serial.print(nextMillis - currentMillis);
    Serial.println();
    lastLogMillis = currentMillis;
  }
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