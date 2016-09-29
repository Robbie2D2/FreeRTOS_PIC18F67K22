/* 
 * File:   main.h
 * Author: Roberto
 *
 * Created on September 29, 2016, 3:52 PM
 */
/* 
 * File:   main.h
 * Author: Roberto
 *
 * Created on September 27, 2016, 12:34 PM
 */

//#include <p18f67k22.h>

#define U5_ENA              LATFbits.LATF2
#define LED_POWER_ON()      LATEbits.LATE0 = 0
#define LED_POWER_OFF()     LATEbits.LATE0 = 1
#define LED_POWER_TOGGLE()  LATEbits.LATE0 = ~LATEbits.LATE0
#define LED_MANUAL_ON()     LATEbits.LATE1 = 0
#define LED_MANUAL_OFF()    LATEbits.LATE1 = 1
#define LED_MANUAL_TOGGLE()  LATEbits.LATE1 = ~LATEbits.LATE1
#define LED_AUTO_ON()       LATEbits.LATE2 = 0
#define LED_AUTO_OFF()      LATEbits.LATE2 = 1
#define LED_AUTO_TOGGLE()  LATEbits.LATE2 = ~LATEbits.LATE2
#define LED_NIVEL_ON()      LATEbits.LATE3 = 0
#define LED_NIVEL_OFF()     LATEbits.LATE3 = 1
#define LED_NIVEL_TOGGLE()  LATEbits.LATE3 = ~LATEbits.LATE3
#define LED_MOTOR_ON()      LATEbits.LATE4 = 0
#define LED_MOTOR_OFF()     LATEbits.LATE4 = 1
#define LED_MOTOR_TOGGLE()  LATEbits.LATE4 = ~LATEbits.LATE4
#define LED_WARNING_ON()    LATEbits.LATE5 = 0
#define LED_WARNING_OFF()   LATEbits.LATE5 = 1
#define LED_WARNING_TOGGLE()  LATEbits.LATE5 = ~LATEbits.LATE5
#define GET_LED_WARNING()   LATEbits.LATE5


#ifndef MAIN_H
#define	MAIN_H


#endif	/* MAIN_H */