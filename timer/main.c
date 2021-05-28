#include "nrf.h"
#include "timer.c"

uint32_t count;

void timer_callback(void)
{
    ++count;
}

int main(void)
{	
	
	start(509*1000, timer_callback);     //start for timer initialization and configuration.
	
	while(1) 
	{
             
		__WFE(); // Wait For Event
		
	}
}
