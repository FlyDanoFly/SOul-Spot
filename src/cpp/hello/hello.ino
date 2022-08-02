// Testing how fmod works on computer vs arduino

// #define myprint(x)
// #define myprintln(x)
#define myprint Serial.print
#define myprintln Serial.println

void setup()  {
  // Serial.begin(9600);
  // Serial.begin(115200);
 Serial.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(PIN7, OUTPUT);
    Serial.println("Let the world just pass me by");

    Serial.print("Let the world just pass me by");
    const float fdenom = 5.0;
    for (float i = -10.0; i < 10.0; i += 0.2) {
        float result = fmod(i, fdenom);
        myprint("fmod(");
        myprint(i);
        myprint(", ");
        myprint(fdenom);
        myprint(") = ");
        myprint(result);
        myprintln("");
    }
}

void loop() {
    Serial.println("--- START OF LOOP ---");
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(PIN7, HIGH);
    delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(PIN7, LOW);
    delay(1000);
    return;
}
