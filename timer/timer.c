#include "nrf.h"

	
void start(uint32_t us, void (*fp)(void));
void stop(void);
void (*callbackfunction)(void);



void start(uint32_t us, void (*fp)(void))
{		
		
    int prescaler = 3, counter_value=0;  //
    
    
	NRF_TIMER2->MODE = 0;                                   // Set the timer mode by setting [0:0] in [1:0]bits in Timer2 mode register
    NRF_TIMER2->TASKS_CLEAR = 1;                            // clear the task first to be usable for later
	NRF_TIMER2->BITMODE = 3;	                              // Set counter to 32 bit resolution , set [1:1] in [1:0]bits
	  NRF_TIMER2->PRESCALER = prescaler;                      // prescalar value=3 (16MHz / 2^3)=>2MHz -->0.5Us
    // for every 0.5micro second , counter increment by one 
	  //  for 1microsec timer count twice  
		counter_value= us*2;
		NRF_TIMER2->CC[0] = counter_value;                      // counter value in counter register 
		NRF_TIMER2->INTENSET = (1<<16);                         // set '1' in 16 bit to Enable interrupt for COMPARE[0] event
    NVIC_EnableIRQ(TIMER2_IRQn);	                          
    NRF_TIMER2->TASKS_START = 1;                            // Start TIMER2
		callbackfunction = fp;
	
}
		

void stop(void)
{
   callbackfunction = 0;
   NRF_TIMER2->TASKS_STOP = 1;          // Stop the timer
   NRF_TIMER2->TASKS_SHUTDOWN = 1;      // Shutdown the timer module
   NVIC_DisableIRQ(TIMER2_IRQn);        // Disable the timer2 interrupt
}


// TIMTER2 interrupt handler.
void TIMER2_IRQHandler(void)
{
  if ((NRF_TIMER2->EVENTS_COMPARE[0] != 0) && ((NRF_TIMER2->INTENSET & (1<<16)) != 0)) //checking compare[0](16th bit) event occur or not
  {
      NRF_TIMER2->EVENTS_COMPARE[0] = 0;           //Clear compare register 0 event	
      callbackfunction(); //calling Timer_callback fuction.
      
  }
}
