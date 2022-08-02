#include <stdio.h>
#include <math.h>


void main(int argc, char **argv) {
    const int arrow_length = 5;
    const float arrow_min = 0.2;
    const float arrow_max = 1.0;

    const int num_leds = 15;
    float offset = 0.0f;
    float rate = 0.13f;
    for (int i = 0; i < 50; i++) {
        printf("Offset %f:\n", offset);
        for (int led = 0; led < num_leds; led++) {
            float r = led - offset;
            float moded = fmod(r, arrow_length);
            float intensity = -1.0;
            if (moded < 0.0) {
                printf("  - %d %f %f %f", led, moded, arrow_length + moded);
                moded = arrow_length + moded;
            }
            if (moded < 1.0) {
                printf("moded < 1.0");
                intensity = moded * arrow_min + (1-moded) * arrow_max;
            }
            else if (moded > arrow_length) {
                printf("ERROR ERROR: TOO LONG %d %f %f\n", led, moded, arrow_length);
            }
            else {
                printf("moded > 1.0");
                intensity = (moded - 1.0) * (arrow_max - arrow_min) / arrow_length + arrow_min;
            }
            printf("    [%03d] %f %f %f\n", led, r, moded, intensity);
        }

        offset += rate;
    }
}