#ifndef __PROGRAM_00_H__
#define __PROGRAM_00_H__

//
// Pulse
//
#include <Wire.h>
#include "../common.h"
#include "../black_lights.h"
#include "program_common.h"


int const DEFAULT_INTENSITY = 0;
int const DEFAULT_LOW_INTENSITY = 1;
int const DEFAULT_HIGH_INTENSITY = 200;
int const DEFAULT_ASCENDING_MILLIS = 20000;
int const DEFAULT_HOLD_HIGH_MILLIS = 10000;
int const DEFAULT_DESCENDING_MILLS = 10000;
int const DEFAULT_HOLD_LOW_MILLIS = 60000;


class Pulse : public Program {
private:
    enum PULSE_STATE {
        PULSE_STATE_INIT,
        PULSE_STATE_ASCENDING,
        PULSE_STATE_HOLD_HIGH,
        PULSE_STATE_DESCENDING,
        PULSE_STATE_HOLD_LOW,
    };
    
    BLACK_LIGHT blackLights[BLACK_LIGHT_COUNT];

    unsigned int currentIntensity;
    PULSE_STATE currentState;
    unsigned int currentAccumulatedMillis;
    unsigned int nextTransitionMillis;

    unsigned int lowIntensity;
    unsigned int highIntensity;
    unsigned int ascendingMillis;
    unsigned int holdHighMillis;
    unsigned int descendingMillis;
    unsigned int holdLowMillis;

    // returns true if the state changes
    bool _update_state() {
        PULSE_STATE previousState = currentState;

        while (true) {
            // print(currentAccumulatedMillis);
            // print(" > ");
            // print(nextTransitionMillis);
            // print(" > ");
            // print(currentAccumulatedMillis < nextTransitionMillis);
            // print(" >> ");
            // print(millis());
            // print_ln("");
            if (currentAccumulatedMillis < nextTransitionMillis) {
                if (previousState != currentState) {
                    print("Pulse: state changed from ");
                    print(previousState);
                    print(" to ");
                    print_ln(currentState);
                }
                return (previousState != currentState);
            }

            switch (currentState) {
                case PULSE_STATE_ASCENDING:
                    currentAccumulatedMillis -= ascendingMillis;
                    nextTransitionMillis = holdHighMillis;
                    currentState = PULSE_STATE_HOLD_HIGH;
                    break;
                case PULSE_STATE_HOLD_HIGH:
                    currentAccumulatedMillis -= holdHighMillis;
                    nextTransitionMillis = descendingMillis;
                    currentState = PULSE_STATE_DESCENDING;
                    break;
                case PULSE_STATE_DESCENDING:
                    currentAccumulatedMillis -= descendingMillis;
                    nextTransitionMillis = holdLowMillis;
                    currentState = PULSE_STATE_HOLD_LOW;
                    break;
                case PULSE_STATE_HOLD_LOW:
                    currentAccumulatedMillis -= holdLowMillis;
                    nextTransitionMillis = ascendingMillis;
                    currentState = PULSE_STATE_ASCENDING;
                    break;
                default:
                    print("EXCEPTION: Invalid state:");
                    print_ln(currentState);
                    break;
            }
        }
    }

    void _handle_state_ascending(unsigned int deltaMillis) {
        // print(currentAccumulatedMillis);
        // print(" > ");
        // print(nextTransitionMillis);
        // print(" > ");
        // print(lowIntensity);
        // print(" > ");
        // print(highIntensity);
        // print(" > ");
        // print((highIntensity - lowIntensity));
        // print(" > ");
        // print(((unsigned long) (highIntensity - lowIntensity) * currentAccumulatedMillis));
        // print(" > ");
        // print((unsigned long) (highIntensity - lowIntensity) * currentAccumulatedMillis / nextTransitionMillis);
        // print(" > ");
        currentIntensity = lowIntensity + (unsigned long) (highIntensity - lowIntensity) * currentAccumulatedMillis / nextTransitionMillis;
        // print(currentIntensity);
        // print_ln("");
        currentIntensity = constrain(currentIntensity, lowIntensity, highIntensity);

        _update_state();
    }

    void _handle_state_hold_high(int deltaMillis) {
        currentIntensity = highIntensity;

        _update_state();
    }

    void _handle_state_descending(int deltaMillis) {
        currentIntensity = highIntensity - (unsigned long) (highIntensity - lowIntensity) * currentAccumulatedMillis / nextTransitionMillis;
        currentIntensity = constrain(currentIntensity, lowIntensity, highIntensity);

        _update_state();
    }

    void _handle_state_hold_low(int deltaMillis) {
        currentIntensity = lowIntensity;

        _update_state();
    }

    void _clear_black_lights() {
        for (int i = 0; i < BLACK_LIGHT_COUNT; i++) {
            blackLights[i] = BLACK_LIGHT_UNDEFINED;
        }
    }

public:
    Pulse() :
        currentIntensity(DEFAULT_LOW_INTENSITY),
        currentState(PULSE_STATE_ASCENDING),
        currentAccumulatedMillis(0),
        nextTransitionMillis(DEFAULT_ASCENDING_MILLIS),
        lowIntensity(DEFAULT_LOW_INTENSITY),
        highIntensity(DEFAULT_HIGH_INTENSITY),
        ascendingMillis(DEFAULT_ASCENDING_MILLIS),
        holdHighMillis(DEFAULT_HOLD_HIGH_MILLIS),
        descendingMillis(DEFAULT_DESCENDING_MILLS),
        holdLowMillis(DEFAULT_HOLD_LOW_MILLIS) {
        _clear_black_lights();
    }

    bool add_black_light(BLACK_LIGHT blackLight) {
        // Find an empty slot and add it to the array
        for (int i = 0; i < BLACK_LIGHT_COUNT; i++) {
            if (blackLights[i] == blackLight) {
                // Already added
                print("Already added ");
                print(i);
                print(" > ");
                print_ln(blackLights[i]);
                return true;
            } else if (blackLights[i] == BLACK_LIGHT_UNDEFINED) {
                print("Adding ");
                print(i);
                print(" > ");
                print(blackLights[i]);

                // Empty slot found, add it
                blackLights[i] = blackLight;
                print(" > ");
                print_ln(blackLights[i]);
                return true;
            }
        }
        print("Cant find match ");
        return false;
    }


    unsigned int _read_uint() {
        unsigned char high = Wire.read();
        unsigned char low = Wire.read();
        return (high << 8) | low;
    }

    int read_data(int bytesReceived) {
        lowIntensity = Wire.read();
        highIntensity = Wire.read();
        ascendingMillis = _read_uint();
        holdHighMillis = _read_uint();
        descendingMillis = _read_uint();
        holdLowMillis = _read_uint();
        print("Pulse::read_data(): ");
        print("params low high asc holdHigh desc holdLow: ");
        print(lowIntensity);
        print(" ");
        print(highIntensity);
        print(" ");
        print(ascendingMillis);
        print(" ");
        print(holdHighMillis);
        print(" ");
        print(descendingMillis);
        print(" ");
        print(holdLowMillis);
        print_ln("");
        return 5;
    }

    void update(int deltaMillis) {
        // print(" --> ");
        // print(currentAccumulatedMillis);
        // print(" > ");
        // print(deltaMillis);
        // print(" > ");
        currentAccumulatedMillis += deltaMillis;
        // print_ln(currentAccumulatedMillis);

        switch (currentState) {
            case PULSE_STATE_ASCENDING:
                _handle_state_ascending(deltaMillis);
                break;
            case PULSE_STATE_HOLD_HIGH:
                _handle_state_hold_high(deltaMillis);
                break;
            case PULSE_STATE_DESCENDING:
                _handle_state_descending(deltaMillis);
                break;
            case PULSE_STATE_HOLD_LOW:
                _handle_state_hold_low(deltaMillis);
                break;
            default:
                print("EXCEPTION: Invalid state:");
                print_ln(currentState);
                break;
        }
    };

    void render() {
        // print("Set blacklight ");
        // print(" to ");
        if (currentIntensity < lowIntensity)
        print_ln(currentIntensity);
        for (int i = 0; i < BLACK_LIGHT_COUNT; i++) {
            if (blackLights[i] == BLACK_LIGHT_UNDEFINED) {
                // print("Done counting black lights at i=");
                // print_ln(i);
                return;
            }

            set_blacklight(blackLights[i], currentIntensity);
        }
    }
};


#endif // __PROGRAM_00_H__