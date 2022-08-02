#ifndef _COMMON_H_
#define _COMMON_H_


#define DEBUG 1


#ifdef DEBUG

#define print(_x) Serial.print(_x)
#define print_ln(_x) Serial.println(_x)

#else

#define print(_x)
#define print_ln(_x)

#endif // DEBUG


#endif // _COMMON_H_
