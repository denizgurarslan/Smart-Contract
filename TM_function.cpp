#include <stdio.h>
#include <math.h>
int main()
{
	int a,i,n,x,result;
	int coefficients[a];
	printf("Enter the degree of the polynomial: ");
	scanf("%d",&a);
	n=0;
	for(i=a;i>=0;i--)
	{
		printf("Enter the coefficient of  x^%d : \n",i);
		scanf("%d",&coefficients[n]);
		n++;
	}
	printf("Enter the input x: \n");
	scanf("%d",&x);
	result=0;
	for(i=0;i<=a;i++)
	{
		result=result+coefficients[i]*pow(x,a-i);

	}
	printf("Result is: %d",result);
	return 0;
}

