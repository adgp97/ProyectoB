#include <stdio.h>
#include "fdacoeffs_char.h"

//#define BUFFER_LEN h_len/2 + h_len%2				//FIR coefficients are symmetrical so only half of them are needed

#define BUFFER_LEN h_len							//Buffer size and filter order
#define SIGNAL_DATA 1001							//PC test. Number of elements in test file

void loadSignal(char *signal);						//PC test. Function that loads a file content into an array

int main(){


	char x[BUFFER_LEN];								//Buffer vector definition

	char signal[SIGNAL_DATA];						//PC test. Initialization of the array that contains data from file
	loadSignal(signal);								//PC test. Call to function tha loads the file data into the array
	int indice = 0;									//PC test. Index initialization
	FILE *result;									//PC test. 
	result = fopen("y_int.txt","a");				//PC test. Creating and opening file to store the results in

//**********************************************************************************************************************************************************************
	//Case 1: buffer is being loaded for the first time
	int i = 0;
	for (int offset = 0; offset < BUFFER_LEN; offset++){													
		//This loop loads the samples into the buffer

		x[offset] = signal[indice];					//PC test. The signal is "sampled" and stored in the buffer
		indice++;									//PC test. Input signal index is incremented by one which "sets the next sample"
													//The final value is index = offset + 1

		//getvalue ----> x[offset]					//Codewarrior. Actual sampling
		
		int i = 0;
		int y = 0;									//PC test. In the final version (microcontroller) "y" will be type int

		for (int k = offset; k >= 0; k--){
			//This loop calculates the convolution given that the buffer is not full
			y += h[k]*x[i];
			i++;			
		}

		fprintf(result,"%d\n",y);					//PC test. Prints the result to y.txt
		//sendblock ----> y 						//Codewarrior. Serial communication with PC
	}


//*******************************************************************************************************************
	//Case 2: The buffer is full

	int offset = 0;
	//for(;;){ 										//Codewarrior
	do{												//PC test. In the final version (microcontroller) this will be an infinite "for"
		//This loop overwirtes the oldest data stored with the new samples

		x[offset] = signal[indice];					//PC test. The signal is "sampled" and stored in the buffer
		indice++;									//PC test. Input signal index is incremented by one which "sets the next sample"

		//getval ----> x[offset]					//Codewarrior. Actual sampling 

		offset = (offset==(BUFFER_LEN-1) ? 0:offset + 1);

		int y = 0;								//PC test. In the final version (microcontroller) "y" will be type int

		for (int k = BUFFER_LEN - 1; k >= 0; k--){
			//This loop calculates the convolution given that the buffer is full

			y += h[k]*x[offset];
			offset = (offset==(BUFFER_LEN-1) ? 0:offset + 1);	//This checks if the offset is at the end of the array. If so, it resets. If not, it is incremented by one
		}


		fprintf(result, "%d\n", y);					//PC test. Prints the result to y.txt 
		//sendblock ----> y 						//Codewarrior. Serial communication with PC
	}while(indice != SIGNAL_DATA);					//PC test. In the final version (microcontroller) this will be an infinite "for"


	fclose(result);									//PC test. Closing file

	getchar();										//PC test
	return 0;										//PC test
}


//******************************************************************************************************
//This function loads the test signal from x.txt file
void loadSignal(char *signal){

	FILE *fptr;

	fptr = fopen("x_char.txt", "r");

	int index = 0;

	do{
	fscanf(fptr,"%c", &signal[index]);
	printf("%c\n",signal[index] );
	index++;
	}while(!feof(fptr));

	fclose(fptr);

	//Zero padding
	for (index; index < SIGNAL_DATA; index++)
	{
		signal[index] = 0;
	}
}
