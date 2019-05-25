// This file is for testing the acquisicion of char data from a txt file


/* The conclusion after executing this file is that, when wanting to scan and store numbers from txt file,
first they have to be scanned as an integer and then casted as char. Scanning directly as char will scan 
each character separately including the space character, e.g. if the number is -23, it will be scanned as
'-' '2' '3'. 
After the integer is casted to char, operations can be performed over the char variable/data as usual
*/
#include <stdio.h>


int main(){

	FILE *x_char,*x_char_clone;

	x_char = fopen("x_char.txt", "r");
	x_char_clone = fopen("x_char_clone.txt", "a");

	int num;

	do{
	fscanf(x_char,"%d", &num);

	char a = (char)num;
	char a2 = a+5;
	int num2 = (int)a2;
	int num3 = (int)a;

	printf("%c\n", a);
	printf("%d\n", a2);
	fprintf(x_char_clone, "%d\n", num3);
	
	}while(!feof(x_char));
	
	fclose(x_char);
	fclose(x_char_clone);
	getchar();


	return 0;
}