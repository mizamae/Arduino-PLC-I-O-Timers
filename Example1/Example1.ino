
// THIS EXAMPLE SHOWS HOW TO "FILTER" A DIGITAL INPUT, NOT CONSIDERING IT HIGH
// UNTIL IT HAS BEEN HIGH FOR 200 CYCLES. THIS CAN BE VERY USEFULL TO DISCARD
// JITTER OR NOISE IN THE INPUTS.

#include <PLCTimer.h>

PLCTimer TC1(TEMP_CON);
PLCTimer TC2(TEMP_CON);
PLCTimer TD1(TEMP_DESCON);
PLCTimer TCD1(TEMP_CON_DESCON);

void setup()
{
  Serial.begin(9600);  
   TC1.Delay  = 2;
   TC2.Delay  = 5;
   TD1.Delay  = 3;
   TCD1.Delay  = 5;
   
  pinMode(2, INPUT);       // sets the digital pin as input
  digitalWrite(2, HIGH);   // turn on pullup resistors
  
   cli();//stop interrupts
  // TIMER 1 CONFIGURATION FOR INTERRUPTING EACH SECOND (1Hz) 
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  sei();//allow interrupts
  
  enable_T1_interrupt();
}

void enable_T1_interrupt()
{
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
}
void disable_T1_interrupt()
{
  // disables timer compare interrupt
  TIMSK1 &= (0 << OCIE1A);
}

ISR(TIMER1_COMPA_vect)    // interrupcion del timer 1
{  
  TC1.Execute();
  TC2.Execute();
  TCD1.Execute();
  TD1.Execute();
}


  
void loop()
{
  static boolean TC1_ant  = false;
  static boolean TC2_ant  = false;
  static boolean TCD1_ant  = false;
  static boolean TD1_ant  = false;
  static boolean in_ant = true;
  static unsigned long mseg_act, mseg_des;
      
  TC1.In     = (digitalRead(2)==LOW);
  TC2.In     = (digitalRead(2)==LOW);
  TD1.In     = (digitalRead(2)==LOW);
  TCD1.In     = (digitalRead(2)==LOW);
  
  if ((digitalRead(2)==LOW) && (in_ant))
  {
    mseg_act  = millis();
  }
  if ((digitalRead(2)==HIGH) && (!in_ant))
  {
    mseg_des  = millis();
  }
  in_ant  = digitalRead(2);
  
  if ((TC1.OUT)&&(!TC1_ant))
  { 
     Serial.print("TC1 a ON en ");
     Serial.print(millis()-mseg_act);
     Serial.println(); 
  }
  if ((!TC1.OUT)&&(TC1_ant))
  { 
     Serial.print("TC1 a OFF en ");
     Serial.print(millis()-mseg_des);
     Serial.println(); 
  }
  
  if ((TC2.OUT)&&(!TC2_ant))
  { 
     Serial.print("TC2 a ON en ");
     Serial.print(millis()-mseg_act);
     Serial.println(); 
  }
  if ((!TC2.OUT)&&(TC2_ant))
  { 
     Serial.print("TC2 a OFF en ");
     Serial.print(millis()-mseg_des);
     Serial.println(); 
  }
  
  if ((TCD1.OUT)&&(TCD1.OUT!=TCD1_ant))
  { 
     Serial.print("TCD a ON en ");
     Serial.print(millis()-mseg_act);
     Serial.println(); 
  }
  if ((!TCD1.OUT)&&(TCD1.OUT!=TCD1_ant))
  { 
     Serial.print("TCD a OFF en ");
     Serial.print(millis()-mseg_des);
     Serial.println(); 
  }
  
  if ((TD1.OUT)&&(!TD1_ant))
  { 
     Serial.print("TD1 a ON en ");
     Serial.print(millis()-mseg_act);
     Serial.println(); 
  }
  if ((!TD1.OUT)&&(TD1_ant))
  { 
     Serial.print("TD1 a OFF en ");
     Serial.print(millis()-mseg_des);
     Serial.println(); 
  }
  
  TC1_ant  = TC1.OUT;
  TC2_ant  = TC2.OUT;
  TCD1_ant = TCD1.OUT;
  TD1_ant  = TD1.OUT;
//    
}

