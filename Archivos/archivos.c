#include <stdio.h>


int main(){

	FILE *x,*x_clone;

	x = fopen("x.txt", "r");
	x_clone = fopen("x_clone.txt", "a");

	float num;

	do{
	fscanf(x,"%f", &num);
	fprintf(x_clone, "%f\n", num);
	}while(!feof(x));
	
	fclose(x);
	fclose(x_clone);
	getchar();
	return 0;
}