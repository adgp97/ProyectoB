#include <stdio.h>
#include "fdacoeffs3.h"

#define BUFFER_LEN h_len							//Tamaño del buffer y orden del filtro
#define SIGNAL_DATA 1001							//Prueba en C. Cantidad de datos en el archivo de prueba

void loadSignal(float *signal);						//Prueba en C. Funcion para cargar data de la se;al original de archivo

int main(){

	float x[BUFFER_LEN];							//Definiciones de las variables de la convolucion

	float signal[SIGNAL_DATA];						//Prueba en C. Inicializacion del arreglo que contiene los datos del archivo
	loadSignal(signal);								//Prueba en C. Cargar los datos del archivo en el arreglo signal
	int indice = 0;									//Prueba en C. Indice de los datos de la se;al del archivo
	FILE *result;									//Prueba en C. 
	result = fopen("y.txt","a");					//Prueba en C. Abrir el archivo para guardar los resultados

//**********************************************************************************************************************************************************************//
	//caso cuando se esta llenando el buffer por primera vez
	int i = 0;
	for (int offset = 0; offset < BUFFER_LEN; offset++)	//Este loop se encarga de agregar las
	{													//muestras al buffer

		x[offset] = signal[indice];					//Prueba en C. "Muestreo" de la señal y almacenamiento en el buffer
		indice++;									//Prueba en C. Incremento del indice de la se;al de entrada, equivalente a muestrear la siguiente vez
													//El valor final es indice = offset + 1
		//getvalue ----> x[offset]					//Codewarrior
		
		int i = 0;
		float y = 0;								//Prueba en C. En la version de Codewarrior, y es un entero
		for (int k = offset; k >= 0; k--)			//Este loop hace la convolucion
		{
			y += h[k]*x[i];
			i++;			
		}

		fprintf(result,"%f\n",y);					//Prueba en C. Imprimir el resultado en el archivo
		//sendblock ----> y 						//Codewarrior
	}


//*******************************************************************************************************************//
	//Caso cuando el buffer ya esta lleno
	int offset = 0;
	//for(;;){ 										//Codewarrior
	do 												//Este loop se encarga de sobreescribir
	{												//datos del buffer al agregar muestras nuevas

		x[offset] = signal[indice];					//Prueba en C. "Muestreo" de la señal y almacenamiento en el buffer
		indice++;									//Prueba en C. Incremento del indice de la se;al de entrada, equivalente a muestrear la siguiente vez

		//getval ----> x[offset]					//Codewarrior

		offset = (offset==(BUFFER_LEN-1) ? 0:offset + 1);

		float y = 0;								//Prueba en C. En la version de Codewarrior, y es un entero

		for (int k = BUFFER_LEN - 1; k >= 0; k--)	//Este loop hace la convolucion
		{
			y += h[k]*x[offset];
			offset = (offset==(BUFFER_LEN-1) ? 0:offset + 1);
		}


		fprintf(result, "%f\n", y);					//Prueba en C. 
		//sendblock ----> y 						//Codewarrior
	}while(indice != SIGNAL_DATA);					//Prueba en C. La version de Codewarrior es un for infinito


	fclose(result);									//Prueba en C. Cerrar el archivo
	getchar();										//Prueba en C
	return 0;										//Prueba en C
}


//******************************************************************************************************
//Cargar la señal desde un archivo para hacer pruebas del algoritmo
void loadSignal(float *signal){

	FILE *fptr;

	fptr = fopen("x.txt", "r");

	int index = 0;

	do{
	fscanf(fptr,"%f", &signal[index]);
	index++;
	}while(!feof(fptr));

	fclose(fptr);

	//Zero padding
	for (index; index < SIGNAL_DATA; index++)
	{
		signal[index] = 0;
	}
}
