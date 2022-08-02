#include <FastLED.h>

#define Num_Strip 150
#define Strip1_PIN 6
#define Strip2_PIN 8

CRGB Strip1[Num_Strip];
CRGB Strip2[Num_Strip];

const int delayMillis = 5;

int colour = 255;
int fading = 255;
int brightness = 255;


const float dD = 15;  
const float min = 0.1;
const float max = 1.0;
const int max_col = 2;
const float max_min = max - min;
float offset = 0.0;
const float rate = 0.0030;
unsigned long prevMillis;
unsigned long startMillis;
unsigned long output_its = 100;
unsigned long output_i = 0;

// #define myprint(x)
// #define myprintln(x)
#define myprint Serial.print
#define myprintln Serial.println


void setup()  {
  // Serial.begin(9600);
  Serial.begin(115200);

//  FastLED.setMaxPowerInVoltsAndMilliamps(5,50); 
  FastLED.addLeds<WS2812B,Strip1_PIN,GRB>(Strip1, Num_Strip);
  FastLED.addLeds<WS2812B,Strip2_PIN,GRB>(Strip2, Num_Strip);
//   FastLED.addLeds<NEOPIXEL,6>(Strip1, Num_Strip);
    prevMillis = millis();
    startMillis = millis();
    Serial.print("Let the world just pass me by");
    // const float fdenom = 5.0;
    // for (float i = -10.0; i < 10.0; i += 0.2) {
    //     float result = fmod(i, fdenom);
    //     myprint("fmod(");
    //     myprint(i);
    //     myprint(", ");
    //     myprint(fdenom);
    //     myprint(") = ");
    //     myprint(result);
    //     myprintln("");
    // }
}

float calc_intensity(int led, float offset) {
  float arrow_length = dD;
  float arrow_min = min;
  float arrow_max = max;

            float r = led - offset;
            float moded = fmod(r, arrow_length);
            float intensity = -1.0;
            if (moded < 0.0) {
                // printf("  - %d %f %f %f", led, moded, arrow_length + moded);
                moded = arrow_length + moded;
            }
            if (moded < 1.0) {
                // printf("moded < 1.0");
                intensity = moded * arrow_min + (1-moded) * arrow_max;
            }
            else if (moded > arrow_length) {
                // printf("ERROR ERROR: TOO LONG %d %f %f\n", led, moded, arrow_length);
            }
            else {
                // printf("moded > 1.0");
                intensity = (moded - 1.0) * (arrow_max - arrow_min) / arrow_length + arrow_min;
            }
            // printf("    [%03d] %f %f %f\n", led, r, moded, intensity);
  return intensity;
}

void loop() {
    output_i += 1;
    if ((output_i % output_its) == 0) {
      myprint("Framerate: ");
      myprint(1.0 * output_i / (millis() - startMillis) * 1000);
      myprintln("");
    }

  if (0 && output_i % 3 ) {
    for (int i = 0; i < Num_Strip - 10; i++) {
        Strip1[i] = CHSV(255, 0, 0);
        Strip2[i] = CHSV(255, 0, 0);
    }
      FastLED.show();
    return;
  }
    unsigned long curMillis = millis();
    float deltaMillis = curMillis - prevMillis;
    prevMillis = curMillis;

    offset = (offset + deltaMillis * rate);
    for (int i = 0; i < Num_Strip - 10; i++) {
        float intensity = calc_intensity(i, offset);

        float thingy2 = intensity * max_col;
        int output = constrain((int)thingy2, 0, max_col);
        Strip1[i] = CRGB(0, output, 0);
        Strip2[i] = CRGB(output, 0, 0);
        // if (i == 0) {
        //   myprint(output);
        //   myprint(" ");
        //   myprint(Strip1[i].r);
        //   myprint(" ");
        //   myprint(Strip1[i].g);
        //   myprint(" ");
        //   myprint(Strip1[i].b);
        //   myprintln(";");
        // }
    } 
    FastLED.show();

}