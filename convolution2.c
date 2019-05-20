#include <stdio.h>
//#include <windef.h>
#define SIGNAL_LEN 8
#define KERNEL_LEN 9
#define OUTPUT_LEN SIGNAL_LEN + KERNEL_LEN - 1

void conv(int v1[], int n1, int v2[], int n2, int r[])
{
    for (int n = 0; n < n1 + n2 - 1; n++){
    	r[n] = 0;
        for (int k = 0; k < KERNEL_LEN && n >= k; k++)
            r[n] += (k < n1 ? v1[k] : 0) * (n - k < n2 ? v2[n - k] : 0);

        printf("output[%d] = %d\n",n,r[n]);
    }
}


int main(){

	int signal[SIGNAL_LEN] = {1, 2, 3, 4, 5, 6, 7, 8};
	int kernel[KERNEL_LEN] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	int output[OUTPUT_LEN];

	conv(signal,SIGNAL_LEN,kernel,KERNEL_LEN,output);
	getchar();
	return 0;
}