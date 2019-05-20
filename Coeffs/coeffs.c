#include <stdio.h>
#include "fdacoefs.h"

//testing the way coefficients are capture from Matlab generated header
//it will print the coefficients to a .txt file
int main(){

	FILE *fptr;

	fptr = fopen("coeffs.txt","a");

	for (int i = 0; i < BL; i++)
	{
		fprintf(fptr, "%Lf\n",B[i]);
	}

	fclose(fptr);

	getchar();
	return 0;
}