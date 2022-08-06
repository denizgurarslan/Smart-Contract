#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
char *encode_table ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
char *encode(char *data,size_t input_length);
char *encode(char *data,size_t input_length)
{
	int mod[]={0,2,1};
	size_t output_length;
	output_length =((input_length+2)/3)*4;

	unsigned char *encoded_data = (unsigned char*) malloc(output_length);

	int i,j;

	for(i=0,j=0; i<input_length;)
	{
		uint32_t firstoctet= i<input_length ? (unsigned char) data[i++]:0 ;

		uint32_t secondoctet= i<input_length ? (unsigned char) data[i++]:0;

		uint32_t thirdoctet= i<input_length ? (unsigned char) data[i++]:0;

		uint32_t triple= (firstoctet<<16)+(secondoctet<<8)+thirdoctet;

		encoded_data[j++]=encode_table[(triple>>3*6) & 0x3F];
		encoded_data[j++]=encode_table[(triple>>2*6) & 0x3F];
		encoded_data[j++]=encode_table[(triple>>1*6) & 0x3F];
		encoded_data[j++]=encode_table[(triple>>0*6) & 0x3F];

	}
	for(i=0;i<mod[input_length % 3];i++)
	{
		encoded_data[output_length-1]='=';
		encoded_data[output_length-2]='=';
	}
	encoded_data[output_length]='\0';
	return encoded_data;
}
int main(void)
{
	char *data="message";  //Enter the message here

	size_t input_length=strlen(data);

    char *encoded_data = encode(data,input_length);
	printf("\n%s",encoded_data);

	return 0;
}
