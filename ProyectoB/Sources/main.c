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
#include "PTD3.h"
#include "PTE6.h"
#include "TI1.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

int h_len = 101;
char h[101] = {
      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,    0,
     -1,   -1,    0,    1,    0,   -1,   -1,    1,    2,    1,   -2,   -2,
      1,    3,    1,   -3,   -3,    1,    4,    1,   -3,   -4,    1,    5,
      2,   -4,   -4,    2,    5,    2,   -5,   -5,    2,    6,    2,   -5,
     -5,    2,    6,    2,   -5,   -5,    2,    6,    2,   -5,   -5,    2,
      5,    2,   -4,   -4,    2,    5,    1,   -4,   -3,    1,    4,    1,
     -3,   -3,    1,    3,    1,   -2,   -2,    1,    2,    1,   -1,   -1,
      0,    1,    0,   -1,   -1,    0,    1,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0};
int samp_flg = 0;		//Sampling flag. 

/* User includes (#include below this line is not maintained by Processor Expert) */

void main(void)
{
  /* Write your local variable definition here */
	char aux, x[101], trama[3];		
	/*
	 * aux 	 : auxiliary variable for storing the sampled data momentarily.
	 * x 	 : buffer for the fir filter.
	 * trama : frame for serial communication.
	 */
	int i, offset, k, y;
	/*
	 * i, k   : auxiliary counters.
	 * offset : auxiliary index for buffer array.
	 * y      : filter output.
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
		AD1_GetChanValue8(0,&aux);		//Converting and storing measured value into aux.
		x[offset] = aux;				//Adding the sampled value to the buffer.
		samp_flg = 0;					//Reset sample flag.
		
		i = 0; y = 0;	
		
		for (k = offset; k >= 0; k--){
			//This loop calculates the convolution given that the buffer is not full.
			y += h[k]*x[i];
			i++;			
		}
		
		//Building the frame.
		trama[0] = 0b0000000000111111 & (y >> 10);
		trama[1] = 0b0000000000011111 & (y >> 5);
		trama[2] = 0b0000000000011111 & y;
		//Digital signals.
		  //D0
		  if(PTA2_GetVal() == 0){
			  trama[0] = trama[1] & 0b11011111;
		  }else {
			  trama[0] = trama[1] | 0b00100000;
		  }
		  //D1
		  if(PTD3_GetVal() == 0){
			  trama[1] = trama[2] & 0b11011111;
		  }else{
			  trama[1] = trama[2] | 0b00100000;
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
  			AD1_GetChanValue8(0,&aux);	//Converting and storing measured value into aux.
  			x[offset] = aux;			//Adding the sampled value to the buffer.
  			samp_flg = 0;				//Reset samplig flag.
  			
  			offset = (offset==(h_len-1) ? 0:offset + 1);	//Checking if the pointer is at the end of buffer.

  			y = 0;	//Reset output value.
  			
  			
  			//Filter activation/deactivation
  			if(PTD3_GetVal() != 0){
  				filt_flg = !filt_flg;	//The state changes when the button is pushed.
  			}
 
  			if(!filt_flg){
  				//Filter OFF.
  				PTC0_ClrVal();			//Filter LED turns OFF.
  				PTE6_SetVal();			//Just for testing purposes.
  				
  	  			//Building the frame.
  				y = aux;
  	  			trama[0] = 0b0000000000111111 & (y >> 10);
  				trama[1] = 0b0000000000011111 & (y >> 5);
  				trama[2] = 0b0000000000011111 & y;
  				//Digital signals.
  				  //D0
  				  if(PTA2_GetVal() == 0){
  					  trama[1] = trama[1] & 0b11011111;
  				  }else {
  					  trama[1] = trama[1] | 0b00100000;
  				  }
  				  //D1
  				  if(PTD3_GetVal() == 0){
  					  trama[2] = trama[2] & 0b11011111;
  				  }else{
  					  trama[2] = trama[2] | 0b00100000;
  				  }
  				//Sending the frame.
  				AS1_SendBlock(trama,sizeof(trama),&ptr);
  				
  			}else{
  				//Filter ON.
  				
  				PTC0_SetVal();			//Filter LED turns ON.
  				PTE6_ClrVal();			//Just for testing purposes.  			

  			for (k = h_len - 1; k >= 0; k--){
  				//This loop calculates the convolution given that the buffer is full.

  				y += h[k]*x[offset];
  				offset = (offset==(h_len-1) ? 0:offset + 1);	//This checks if the offset is at the end of the array. If so, it resets. If not, it is incremented by one.
  			}

  			//Building the frame.
  			trama[0] = 0b0000000000111111 & (y >> 10);
			trama[1] = 0b0000000000011111 & (y >> 5);
			trama[2] = 0b0000000000011111 & y;
			//Digital signals.
			  //D0
			  if(PTA2_GetVal() == 0){
				  trama[0] = trama[1] & 0b11011111;
			  }else {
				  trama[0] = trama[1] | 0b00100000;
			  }
			  //D1
			  if(PTD3_GetVal() == 0){
				  trama[1] = trama[2] & 0b11011111;
			  }else{
				  trama[1] = trama[2] | 0b00100000;
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