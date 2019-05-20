#include <stdio.h>
#include "fdacoeffs.h"

//testing the way coefficients are taken from Matlab generated header
//it will print the coefficients to a .txt file
int main(){

	FILE *fptr;

	fptr = fopen("coeffs.txt","a");

	for (int i = 0; i < BL; i++)
	{
		fprintf(fptr, "%f\n",B[i]);
	}

	fclose(fptr);

	getchar();
	return 0;
}