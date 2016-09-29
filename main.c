#include <p18f67k22.h>
#include <reset.h>
#include "main.h"


#define Nop()		{_asm nop _endasm}
/** CONFIGURATION **************************************************/
/*#pragma config OSC=HSPLL
#pragma config IESO=OFF
#pragma config PWRT=ON
#pragma config WDT=OFF
#pragma config MCLRE=ON
#pragma config XINST=OFF
#pragma config DEBUG=OFF
#pragma config FCMEN = OFF
#pragma config LVP=OFF
#pragma config BOREN=OFF
#pragma config PBADEN = OFF*/
#pragma config RETEN=OFF, INTOSCSEL=LOW, SOSCSEL=LOW, XINST=ON, FOSC=INTIO2
#pragma config PLLCFG=ON, FCMEN=ON, IESO=ON, PWRTEN=ON, BOREN=NOSLP, BORV=2
#pragma config BORPWR=LOW, WDTEN=OFF, WDTPS=512, RTCOSC=SOSCREF, CCP2MX=PORTC //WDTPS=1024 
#pragma config MCLRE=ON, STVREN=ON, CP0=ON, CP1=ON, CP2=ON, CP3=ON,CP4=ON, CP5=ON, CP6=ON, CP7=ON, CPB=OFF
#pragma config CPD=OFF, WRT0=OFF, WRT1=OFF, WRT2=OFF, WRT3=OFF, WRTC=OFF, WRTB=OFF
#pragma config WRTD=OFF, EBRT0=OFF, EBRT1=OFF, EBRT2=OFF, EBRT3=OFF, EBRTB=OFF
/** END CONFIGURATION **************************************************/
#include "FreeRTOS.h"
#include "task.h"
//#include "stdio.h"
#include <usart.h>

#define PRIORITY_TASK0		( tskIDLE_PRIORITY + 1 )
#define PRIORITY_TASK1		( tskIDLE_PRIORITY + 2 )
#define PRIORITY_TASK2		( tskIDLE_PRIORITY + 3 )
#define PRIORITY_TASK3		( tskIDLE_PRIORITY + 3 )
volatile unsigned char BOR_Flag = 0x05;
volatile unsigned char POR_Flag = 0x05;
volatile unsigned char MCLR_Flag = 0x05;
volatile unsigned char WDTWU_Flag = 0x05;
volatile unsigned char WDTTO_Flag = 0x05;
volatile unsigned char WU_Flag = 0x05;
volatile unsigned char RI_Flag = 0x05;
volatile unsigned char CMR_Flag = 0x05;
size_t freeHeap;
volatile unsigned long uIdleCycleCount = 0UL;
volatile unsigned long uLastIdleCycleCount=0UL;
volatile unsigned long uDiffIdleCycleCount=0UL;

/*************************** Tasks Prototypes ****************************/
//static const char *pcTextForTask0 = "Tarea 0\n";
//static const char *pcTextForTask1 = "Tarea 1\n";
//static const char *pcTextForTask2 = "Tarea 2\n";

static void vTASK0( void *pvParameters );
static void vTASK1( void *pvParameters );
static void vTASK2( void *pvParameters );
static void vTASK3( void *pvParameters );

/*-----------------------------------------------------------*/
void main( void ){
    //ResetInfo();
    if (isBOR()) {
        BOR_Flag = 0x0A;
    }
    if (isPOR()) {
        POR_Flag = 0x0A;
    }
    if (isMCLR()) {
        MCLR_Flag = 0x0A;
        //Quitar bandera de log full
        //Inter_EEPROM_Write(332,0x05);
    }
    if (isWDTTO()) {
        WDTTO_Flag = 0x0A;

    }
    if (isWDTWU()) {
        WDTWU_Flag = 0x0A;
    }
    if (isWU()) {
        WU_Flag = 0x0A;
    }
    if (RCONbits.NOT_RI == 0) {
        RCONbits.NOT_RI = 1;
        RI_Flag = 0x0A;
    }
    if (RCONbits.NOT_CM == 0) {
        RCONbits.NOT_CM = 1;
        CMR_Flag = 0x0A;
    }
	//TRISB=0x00;
    PORTE = 0;
    PORTC = 0;
    PORTF = 0;
    
    LATC = 0;
    LATE = 0;
    LATF = 0;
    
    TRISC = 0b10010010;
    TRISE = 0b10000000;
    TRISF = 0x30;
    
    U5_ENA = 0; //1;
    
     // Oscillator Configuration
    OSCCON = 0b01111100; // 64MHz to 16MIPS oscillator configuration
    OSCCON2 = 0b01000010;
    OSCTUNE = 0b11000000;
    Nop();
    Nop();
    
    baud1USART(BAUD_IDLE_TX_PIN_STATE_HIGH & BAUD_16_BIT_RATE & BAUD_WAKEUP_OFF & BAUD_AUTO_OFF & USART_BRGH_HIGH);
    Open1USART(USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX, 277); //baudrate 57600
    TXREG1 = 0;
    printf(" \n");
    printf("inicio\n");
    LED_MANUAL_OFF();
    LED_AUTO_OFF();
    LED_MOTOR_OFF();
    LED_NIVEL_OFF();
    LED_WARNING_OFF();
    LED_POWER_OFF();
    
	vPortInitialiseBlocks();
    
//	xTaskCreate( vTASK0, ( const char * const ) "T0", configMINIMAL_STACK_SIZE, (void*)pcTextForTask0, PRIORITY_TASK0, NULL );
//	xTaskCreate( vTASK1, ( const char * const ) "T1", configMINIMAL_STACK_SIZE, (void*)pcTextForTask1, PRIORITY_TASK1, NULL );
//  xTaskCreate( vTASK2, ( const char * const ) "T2", configMINIMAL_STACK_SIZE, (void*)pcTextForTask2, PRIORITY_TASK1, NULL );
    xTaskCreate( vTASK0, ( const char * const ) "T0", configMINIMAL_STACK_SIZE, NULL, PRIORITY_TASK0, NULL );
	xTaskCreate( vTASK1, ( const char * const ) "T1", configMINIMAL_STACK_SIZE, NULL, PRIORITY_TASK1, NULL );
    xTaskCreate( vTASK2, ( const char * const ) "T2", configMINIMAL_STACK_SIZE, NULL, PRIORITY_TASK2, NULL );
    xTaskCreate( vTASK3, ( const char * const ) "T3", configMINIMAL_STACK_SIZE, NULL, PRIORITY_TASK3, NULL );
    
	vTaskStartScheduler();
    for(;;)
    {
        printf("Error\n");
    }
}
/*-----------------------------------------------------------*/
static void vTASK0( void *pvParameters ){
    //char *pcTaskName=(char*)pvParameters;
    portTickType xLastWakeTime;
    xLastWakeTime=xTaskGetTickCount();
    
	while(1)
	{
		//LATBbits.LATB0=!PORTBbits.RB0;
        //printf(pcTaskName);
        freeHeap = xPortGetFreeHeapSize();
        printf("Tarea0\n");
        uDiffIdleCycleCount=uIdleCycleCount-uLastIdleCycleCount;
        printf("Idle= %lu\n",uDiffIdleCycleCount);
        uLastIdleCycleCount=uIdleCycleCount;
        uIdleCycleCount=0;
        LED_POWER_TOGGLE();
        //printf("freeHead= %d\n",freeHeap);
		//vTaskDelay(250/portTICK_RATE_MS);
        vTaskDelayUntil(&xLastWakeTime,(100/portTICK_RATE_MS));
        
       
	}
}
/*-----------------------------------------------------------*/
static void vTASK1( void *pvParameters ){
    //char *pcTaskName=(char*)pvParameters;
	while(1){
		//LATBbits.LATB1=!PORTBbits.RB1;
        //printf(pcTaskName);
        printf("Tarea1\n");
        LED_MANUAL_TOGGLE();
		vTaskDelay(350/portTICK_RATE_MS);
        
	}
}
/*-----------------------------------------------------------*/
static void vTASK2( void *pvParameters ){
    //char *pcTaskName=(char*)pvParameters;
	while(1){
		//LATBbits.LATB1=!PORTBbits.RB1;
        //printf(pcTaskName);
        printf("Tarea2\n");
        LED_AUTO_TOGGLE();
		vTaskDelay(450/portTICK_RATE_MS);
        
	}
}
static void vTASK3( void *pvParameters ){
    //char *pcTaskName=(char*)pvParameters;
	while(1){
		//LATBbits.LATB1=!PORTBbits.RB1;
        //printf(pcTaskName);
        printf("Tarea3\n");
        LED_NIVEL_TOGGLE();
		vTaskDelay(1000/portTICK_RATE_MS);
        
	}
}
void vApplicationIdleHook(void){
    uIdleCycleCount++;
}