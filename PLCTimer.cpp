

#include "Arduino.h"
#include "PLCTimer.h"


// C, CD, D
PLCTimer::PLCTimer(uint8_t type)
{
	_Type	= type;
	if (_Type	== TEMP_CON)		// CON
	{
		Callback	=	&PLCTimer::TC;
	}else if (_Type	== TEMP_DESCON)	// DESCON
	{
		Callback	=	&PLCTimer::TD;
	}else if (_Type	== TEMP_CON_DESCON)					// CON-DESCON
	{
		Callback	=	&PLCTimer::TCD;
	}else if (_Type	== PULSE)					// PULSE
	{
		Callback	=	&PLCTimer::_PULSE;
	}
}

volatile void PLCTimer::Execute()
{
	(this->*Callback) ();

}


void PLCTimer::TC()
{
	if ((!In) || (Reset))
	{
		_temporiza				= false;
	}else if (!(_In_ant))
	{	
		_timer					= Delay;		// to guarantee it is counting correctly
		_temporiza				= true;
	}

	if (_temporiza)
	{
		_timer				-= 1;

		if (_timer	<=0)
		{
			OUT			= true;
			_temporiza		= false;
		}else
		{
			OUT			= false;
		}
	}else{OUT	= In;}
	_In_ant	= In;
}

void PLCTimer::TD()
{
	if ((In) || (Reset))
	{
		_timer					= Delay;
		_temporiza				= false;
	}else if (_In_ant)
	{	_temporiza= true;}

	if (_temporiza)
	{
		_timer				-= 1;
		
		if (_timer<=0)
		{
			OUT			= false;
			_temporiza		= false;
		}else
		{
			OUT			= true;
		}
	}else{OUT	= In;}
	_In_ant	= In;
}

void PLCTimer::TCD()
{
	static boolean activa, desactiva;
	if ((In)&&(!_In_ant))
	{
		activa	= true;
		desactiva	= false;
		_timer					= Delay;
	}else if ((!In)&&(_In_ant))
	{
		activa	= false;
		desactiva	= true;
		_timer					= Delay;
	}
	
	if (activa)
	{TC();}
	else if (desactiva)
	{TD();}
	else
	{OUT	= In;}
	
}

void PLCTimer::_PULSE()
{
	if ((In) && (!_In_ant))
	{
		_timer					= Delay;
		_temporiza				= true;
	}

	if (_temporiza)
	{
		_timer				-= 1;
		
		if (_timer<=0)
		{
			OUT			= false;
			_temporiza		= false;
		}else
		{
			OUT			= true;
		}
	}else{OUT	= false;}
	_In_ant	= In;
	
}
