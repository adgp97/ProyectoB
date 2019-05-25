/*
This file compares data filtered with Matlab (y_int_test.txt) with data filtered with 
filter_int.c (y_int.txt)
*/

#include <stdio.h>

int main(){
	
	FILE *y_int, *y_int_test;

	y_int = fopen("y_int.txt","r");
	y_int_test = fopen("y_int_test.txt","r");

	int a,b,error,data_num = 0;

	do{

		fscanf(y_int, "%d", &a);
		fscanf(y_int_test, "%d", &b);
		data_num++;

		if (a != b){
			printf("An error was found in element %d\n",data_num);
			printf("Data missmatched is: from y_int: %d; from y_int_test: %d\n",a,b);
			error++;
		}

	}while(!feof(y_int));

	printf("Total error quantity: %d\n",error);

	fclose(y_int);
	fclose(y_int_test);

	getchar();
	return 0;
}