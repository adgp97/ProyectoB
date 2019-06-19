/* ###################################################################
**     Filename    : main.c
**     Project     : ProyectoB
**     Processor   : MC9S08QE128CLK
**     Version     : Driver 01.12
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2019-06-14, 12:39, # CodeGen: 0
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
#include "AD1.h"
#include "AS1.h"
#include "PTA2.h"
#include "PTC0.h"
#include "PTD2.h"
#include "PTE6.h"
#include "TI1.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

int h_len = 11;
char h[11] = {0,  -10,   -8,   12,   39,   51,   39,   12,   -8,  -10,    0};
int samp_flg = 0;		//Sampling flag. 

/* User includes (#include below this line is not maintained by Processor Expert) */

void main(void)
{
  /* Write your local variable definition here */
	char aux1, aux2, x1[11], x2[11], trama[4];		
	/*
	 * aux 	 : auxiliary variable for storing the sampled data momentarily. aux1 for x coordinate, aux2 for y coordinate.
	 * x 	 : buffer for the fir filter. x1 for x coordinate, x2 for y coordinate.
	 * trama : frame for serial communication.
	 */
	int i, offset, k, y1, y2;
	/*
	 * i, k   : auxiliary counters.
	 * offset : auxiliary index for buffer array.
	 * y      : filter output. y1 for x coordinate, y2 for y coordinate.
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
  
//**********************************************************************************************************************************************************************/
//Case 1: buffer is being loaded for the first time.
	//i = 0;
	for (offset = 0; offset < h_len; offset++){													
		//This loop loads the samples into the buffer.
		
		while(samp_flg == 0){}			// Idle while not ready to sample.
		AD1_Measure(TRUE);				//Measure.
		AD1_GetChanValue8(0,&aux1);		//Converting and storing measured value into aux.
		AD1_GetChanValue8(1,&aux2);
		x1[offset] = aux1;				//Adding the sampled value to the buffer.
		x2[offset] = aux2;
		samp_flg = 0;					//Reset sample flag.
		
		i = 0; y1 = 0; y2 = 0;	
		
		for (k = offset; k >= 0; k--){
			//This loop calculates the convolution given that the buffer is not full.
			y1 += h[k]*x1[i];
			y2 += h[k]*x2[i];
			i++;			
		}
		
		//Building the frame.
		trama[0] = 0b0000000000001111 & (y1 >> 4);
		trama[1] = (0b0000000000001111 & y1) | 0b0000000001000000;
		trama[2] = (0b0000000000001111 & (y2 >> 4)) | 0b0000000010000000;
		trama[3] = (0b0000000011001111 & y2) | 0b0000000011000000;
		//Digital signals.
		  //D0
		  if(PTA2_GetVal() == 0){
			  trama[0] = trama[0] & 0b11011111;
		  }else {
			  trama[0] = trama[0] | 0b00100000;
		  }
		  //D1
		  if(PTD2_GetVal() == 0){
			  trama[0] = trama[0] & 0b11101111;
		  }else{
			  trama[0] = trama[0] | 0b00010000;
		  }
		
		//Sending the frame
		AS1_SendBlock(trama,sizeof(trama),&ptr);		
	}

//*******************************************************************************************************************
  		//Case 2: The buffer is full.
	
  		offset = 0;
  		for(;;){ 										
  			//This loop overwirtes the oldest data stored with the new samples.
  			
  			while(samp_flg == 0){}		//Idle while not ready to sample.
  			AD1_Measure(TRUE);			//Measure.
  			AD1_GetChanValue8(0,&aux1);	//Converting and storing measured value into aux.
  			AD1_GetChanValue8(1,&aux2);
  			x1[offset] = aux1;			//Adding the sampled value to the buffer.
  			x2[offset] = aux2;
  			samp_flg = 0;				//Reset samplig flag.
  			
  			offset = (offset==(h_len-1) ? 0:offset + 1);	//Checking if the pointer is at the end of buffer.

  			y1 = 0;	y2 = 0;				//Reset output value.
  			
  			
  			//Filter activation/deactivation
  			if(PTD2_GetVal() == 0){
  				filt_flg = !filt_flg;	//The state changes when the button is pushed.
  			}
 
  			if(!filt_flg){
  				//Filter OFF.
  				PTC0_ClrVal();			//Filter LED turns OFF.
  				PTE6_SetVal();			//Just for testing purposes.
  				
  	  			//Building the frame.
  				y1 = aux1;
  				y2 = aux2;
  				//Building the frame.
  				trama[0] = 0b0000000000001111 & (y1 >> 4);
  				trama[1] = (0b0000000000001111 & y1) | 0b0000000001000000;
  				trama[2] = (0b0000000000001111 & (y2 >> 4)) | 0b0000000010000000;
  				trama[3] = (0b0000000000001111 & y2) | 0b0000000011000000;
  				//Digital signals.
  				  //D0
  				  if(PTA2_GetVal() == 0){
  					  trama[0] = trama[0] & 0b11011111;
  				  }else {
  					  trama[0] = trama[0] | 0b00100000;
  				  }
  				  //D1
  				  if(PTD2_GetVal() == 0){
  					  trama[0] = trama[0] & 0b11101111;
  				  }else{
  					  trama[0] = trama[0] | 0b00010000;
  				  }
  				//Sending the frame.
  				AS1_SendBlock(trama,sizeof(trama),&ptr);
  				
  			}else{
  				//Filter ON.
  				
  				PTC0_SetVal();			//Filter LED turns ON.
  				PTE6_ClrVal();			//Just for testing purposes.  			

  			for (k = h_len - 1; k >= 0; k--){
  				//This loop calculates the convolution given that the buffer is full.

  				y1 += h[k]*x1[offset];
  				y2 += h[k]*x2[offset];
  				offset = (offset==(h_len-1) ? 0:offset + 1);	//This checks if the offset is at the end of the array. If so, it resets. If not, it is incremented by one.
  			}

  			//Building the frame.
  			trama[0] = 0b0000000000001111 & (y1 >> 4);
			trama[1] = (0b0000000000001111 & y1) | 0b0000000001000000;
			trama[2] = (0b0000000000001111 & (y2 >> 4)) | 0b0000000010000000;
			trama[3] = (0b0000000011001111 & y2) | 0b0000000011000000;
			//Digital signals.
			  //D0
			  if(PTA2_GetVal() == 0){
				  trama[0] = trama[0] & 0b11011111;
			  }else {
				  trama[0] = trama[0] | 0b00100000;
			  }
			  //D1
			  if(PTD2_GetVal() == 0){
				  trama[0] = trama[0] & 0b11101111;
			  }else{
				  trama[0] = trama[0] | 0b00010000;
			  }
			//Sending the frame.
			AS1_SendBlock(trama,sizeof(trama),&ptr);
  			}
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
