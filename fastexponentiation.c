#include<stdio.h>    
#include<stdlib.h>
#include <math.h>
#define MAXSIZE 2048
int takepow(int base,int exponent)
{
	if (exponent == 0)
        return 1;
    else if (exponent < 0)
        return 1 / pow(base, -exponent);
    else if (exponent % 2 == 0)
    {
        int half_pow = pow(base, exponent / 2);
        return half_pow * half_pow;
	}
    else
        return base * pow(base, exponent - 1);
}
int makearray(int n)
{	
	int i,A[MAXSIZE];
	for(i=0;i<sizeof(n);i++)
	{
		A[i]=n;
	}
	printf("array: %d\n",*A);  // put the number in an array an called it back..
}
int fastmodular(int exp,int base,int mode)
{
	int binary[MAXSIZE],row[MAXSIZE],i,size,a,b;
	for(i=0;exp>0;i++)    
	{    
		binary[i]=exp%2;    
		exp=exp/2;    
	}    
	size=i;
	printf("\nBinary of Given Number is= ");    
	for(i=i-1;i>=0;i--)    
	{    
		printf("%d ",binary[i]);    
	}
	printf("\nSize of the array is= %d\n",size);
	//int a=sizeof(binary)/sizeof(binary[0]); //for finding array size
	
	printf("            TABLE                \n-------------------------------------\n");
	int result;
	a=size;
	printf("  ");
	for(a=a-1;a>=0;a--)    
	{    
		printf("%d ",binary[a]);    
	}
	printf("\n---------------------------------------------\n");
	row[0]=base;
	printf("%d ",row[0]);
	i=1;
	b=size;
	for(size=size-2;size>=0;size--)
	{
		if(binary[size]==1)
		{
				row[i]=((takepow(row[i-1],2)%mode)*base)%mode;
				printf("%d ",row[i]);
		}
		else if (binary[size]==0)
		{
			row[i]=takepow(row[i-1],2)%mode;	
			printf("%d ",row[i]);
		}
		i=i+1;
	}
	printf("\n\nANSWER= %d",row[b-1]);
}
int main(){  
	int n,exp,mode;    
	system ("cls");  
	printf("Enter the base: ");    
	scanf("%d",&n);   
	printf("Enter the exponent: ");
	scanf("%d",&exp);
	printf("Enter the mode(q): ");
	scanf("%d",&mode);
	//makearray(n);
	fastmodular(exp,n,mode);
	return 0;  
}  
