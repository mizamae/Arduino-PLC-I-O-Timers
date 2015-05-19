// DEFINICION DE ESTRUCTURAS DE DATOS Y FUNCIONES DE PLC ADAPTADAS PARA ARDUINO

#ifndef PLCTimer_h
#define PLCTimer_h

#include "Arduino.h"

#define TEMP_CON		0
#define TEMP_DESCON		1
#define TEMP_CON_DESCON	2
#define PULSE			3



class PLCTimer
{
typedef void (PLCTimer::*ptrF)(void);	// define the type of pointers to functions void (void)

public:
	PLCTimer(uint8_t type);
	// VARIABLES
	volatile bool In;
	volatile int Delay;
	bool Reset;									// manual reset
	volatile bool OUT;									// salida
	volatile void Execute();

private:
	uint8_t _Type;
	volatile int _timer;									// memory to keep counting the delay
	volatile boolean _temporiza;
	volatile boolean _In_ant;
	ptrF Callback;
	// FUNCTIONS
	void TC(void);
	void TD(void);
	void TCD(void);
	void _PULSE(void);
};



#endif
