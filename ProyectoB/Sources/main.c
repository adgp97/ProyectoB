/* ###################################################################
**     Filename    : main.c
**     Project     : ProyectoB
**     Processor   : MC9S08QE128CLK
**     Version     : Driver 01.12
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2019-06-22, 17:07, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.12
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "TI1.h"
#include "AD1.h"
#include "PTD2.h"
#include "PTA3.h"
#include "PTA2.h"
#include "AS1.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

char h_len = 26;
char h[26] = {-1, 0, 1, -1, -4, -2, 6, 9, -7, -25, -5, 62, 127, 127, 62, -5, -25, -7, 9, 6, -2, -4, -1, 1, 0, -1};
int samp_flg = 0;		//Sampling flag.

/* User includes (#include below this line is not maintained by Processor Expert) */

void main(void)
{
  /* Write your local variable definition here */
	char aux1, aux2, x1[26], x2[26], trama[3], offset1 = 0, offset2 = 0, count = 0;
	char signed k;		
	/*
	 * aux 	  : auxiliary variable for storing the sampled data momentarily. aux1 for x coordinate, aux2 for y coordinate.
	 * x 	  : buffer for the fir filter. x1 for x coordinate, x2 for y coordinate.
	 * trama  : frame for serial communication.
	 * offset : auxiliary index for buffer array.
	 * k      : auxiliary counters.
	 * count  : counter used to estimate the maximum order of filter.
	 */
	int y1, y2, ch_flg = 1;
	/*
	 * y      : filter output. y1 for x coordinate, y2 for y coordinate.
	 * ch_flg : channel flag. 1 for x coordinate, 0 for y coordinate
	 */
	bool  filt_flg = TRUE;
	/*
	 * filt_flg : filter flag. TRUE if filter is ON. FALSE if filter is OFF.
	 */
	word ptr;

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
  for(;;){
	  if(ch_flg == 1){
		  //Sampling and processing of x coordinate
		  while(samp_flg == 0){}		//Idle while not ready to sample.
		  AD1_Measure(TRUE);			//Measure.
		  AD1_GetChanValue8(0,&aux1);	//Converting and storing measured value into aux.
		  x1[offset1] = aux1;			//Adding the sampled value to the buffer.
  			
		  offset1 = (offset1==(h_len-1) ? 0:offset1 + 1);	//Checking if the pointer is at the end of buffer.

		  y1 = 0;				//Reset output value.
  			
		  //Filter activation/deactivation
		  if(PTD2_GetVal() == 0){
			  //The state changes when the button is pushed.
			  filt_flg = !filt_flg;}	
  			
		  if(!filt_flg){
			  //Filter OFF.
			  y1 = aux1;		//Outputs the last sample
		  }else{
			  //Filter ON.
			  for (k = h_len - 1; k >= 0; k--){
				  //This loop calculates the convolution given that the buffer is full.
				  y1 += h[k]*x1[offset1];							//Filtering process
				  offset1 = (offset1==(h_len-1) ? 0:offset1 + 1);	//This checks if the offset is at the end of the array. If so, it resets. If not, it is incremented by one.
			  }		
		  }
		  //Building the frame.
		  trama[0] = ((y1 >> 12) & 0b0000000000111111) | 0b0000000010000000;
		  trama[1] = ((y1 >> 6)  & 0b0000000000111111) | 0b0000000010000000;
		  trama[2] =    (y1      & 0b0000000000111111) | 0b0000000001000000;
		  ch_flg = 0;		//Set to sample y coordinate
  			
  		}else{
  		  //Sampling and processing of y coordinate
  			while(samp_flg == 0){}		//Idle while not ready to sample.
  			AD1_Measure(TRUE);			//Measure.
  			AD1_GetChanValue8(1,&aux2); //Converting and storing measured value into aux.
  			x2[offset2] = aux2;			//Adding the sampled value to the buffer.
  			
  			offset2 = (offset2==(h_len-1) ? 0:offset2 + 1);	//Checking if the pointer is at the end of buffer.
  	
  			y2 = 0;						//Reset output value.
  				
  			//Filter activation/deactivation
  			if(PTD2_GetVal() == 0){
  				//The state changes when the button is pushed.
  				filt_flg = !filt_flg;}	
  			
  			if(!filt_flg){
  				//Filter OFF.
  				y2 = aux2;			//Outputs the last sample
  			}else{
  				//Filter ON.
  				for (k = h_len - 1; k >= 0; k--){
  				//This loop calculates the convolution given that the buffer is full.
  				y2 += h[k]*x2[offset2];							//Filtering process
  				offset2 = (offset2==(h_len-1) ? 0:offset2 + 1);	//This checks if the offset is at the end of the array. If so, it resets. If not, it is incremented by one.
  				}
  			}
  			
  			//Building the frame.
  			trama[0] = (y2 >> 12) | 0b0000000011000000;
  			trama[1] = ((y2 >> 6) & 0b0000000000111111) | 0b0000000010000000;
  			trama[2] =    (y2     & 0b0000000000111111) | 0b0000000001000000;
  			ch_flg = 1;
  		}
  		//Including digital signals.
  		//D0
  		if(PTA2_GetVal() == 0){
  			trama[0] = trama[0] & 0b11011111;
  		}else {
  			trama[0] = trama[0] | 0b00100000;
  		  }
  		//D1
  		if(PTA3_GetVal() == 0){
  			trama[0] = trama[0] & 0b11101111;
  		}else{
  			trama[0] = trama[0] | 0b00010000;
  		}
  		//Sending the frame.
  		AS1_SendBlock(trama,sizeof(trama),&ptr);	//Sending data through serial port
  		samp_flg = 0;		//Reset sample flag
  		count++;			
  }
  

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
