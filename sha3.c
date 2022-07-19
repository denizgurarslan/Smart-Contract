#include "sha3.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

unsigned int sha3_rotConst[5][5] = {
    {0, 36, 3, 105, 210},
    {1, 300, 10, 45, 66},
    {190, 6, 171, 15, 253},
    {28, 55, 153, 21, 120},
    {91, 276, 231, 136, 78}};

unsigned long long sha3_roundConsts[SHA3_NR] = {
    0x0000000000000001,
    0x0000000000008082,
    0x800000000000808A,
    0x8000000080008000,
    0x000000000000808B,
    0x0000000080000001,
    0x8000000080008081,
    0x8000000000008009,
    0x000000000000008A,
    0x0000000000000088,
    0x0000000080008009,
    0x000000008000000A,
    0x000000008000808B,
    0x800000000000008B,
    0x8000000000008089,
    0x8000000000008003,
    0x8000000000008002,
    0x8000000000000080,
    0x000000000000800A,
    0x800000008000000A,
    0x8000000080008081,
    0x8000000000008080,
    0x0000000080000001,
    0x8000000080008008};
unsigned long long sha3_rotWord(unsigned long long w,unsigned int d)
{
    d= d & 0x3f;
    unsigned long long max = ~0l;
    unsigned long long wrapPortion= (max<<(64-d))&w;
    unsigned long long shiftPortion= (max>>d)&w;

    wrapPortion >>= 64-d;
    shiftPortion <<= d;

    return wrapPortion | shiftPortion;
}
void sha3_keccak_f(unsigned long long A[5][5])
{
    int i;
    int x,y;
    for(i=0;i<SHA3_NR;i++)
    {
            //THETA----------
        unsigned long long C[5];
        unsigned long long D[5];

        //calculate C
        for(x=0;x<5;x++)
        {
            C[x]=0L;
            for(y=0;y<5;y++)
            {
                C[x]^= A[x][y];
            }
        }
        //calculate D
        for(x=0;x<5;x++)
        {
            D[x]=C[(x+4)%5] ^ sha3_rotWord(C[(x+1)%5],1);
        }
        //put result into A
        for(x=0;x<5;x++)
        {
            for(y=0;y<5;y++)
            {
                A[x][y]^=D[x];
            }
        }
        //RHO-PI ===================
        unsigned long long B[5][5];
        memset(B,0,5 * 5 * sizeof(unsigned long long));
        for(x=0;x<5;x++)
        {
            for(y=0;y<5;y++)
            {
                B[y][(2*x+3*y) %5]= sha3_rotWord(A[x][y],sha3_rotConst[x][y]);
            }
        }
        //CHI =================
        for(x=0;x<5;x++)
        {
            for(y=0;y<5;y++)
            {
                A[x][y]=B[x][y]^((~B[(x+1) % 5][y]) & B[(x+2) %5][y]);
            }
        }
        //IOTA==================
        A[0][0]^= sha3_roundConsts[i];
    }
}
int sha3_hash(unsigned char *in,int n,int mode, unsigned char **out)
{
    int r,ret_len;

    switch(mode){
    case SHA3_128:
        r=1344;
        break;
    case SHA3_256:
        r=1088;
        break;
    default: //SHA3_512
        mode=SHA3_512;
        r=576;
        break;
    }
    ret_len= mode >> 3; //=>bytes
    r >>= 3;          //=>bytes
    //allocate output
    *out=malloc(ret_len * sizeof(unsigned char));
    //state variables
    unsigned long long A[5][5];
    memset(A,0,5*5*sizeof(unsigned long long));

    int cursor=0;       //cursor in the message
    int noBlocks= n / r + 1; //always pad
    int i;
    //ABSORBÝNG PHASE
    for(i=0;i<noBlocks;i++)
    {
        int noBytesInBlock=MIN(r,MAX(n-cursor,0)); //only take in 0 and r bytes per block
        int noPadding=r-noBytesInBlock;  //number of bytes to pad to get a complete block
        int padIdx=0;       //current number of bytes already padded
        int blockCursor=0;  //cursor in the block
        int x,y;
        for(y=0;y<5;y++)
        {
            for(x=0;x<5;x++)
            {
               unsigned long long tmp=0L;
               if(blockCursor<r)    //have not written the complete block yet
               {
                   int noBytesInWord=MIN(8,MAX(noBytesInBlock-blockCursor,0));
                   if(noBytesInWord)
                   {
                       memcpy(&tmp,in+cursor+blockCursor,noBytesInWord);
                   }
                   //determine if need padding in this word
                   if(noBytesInWord !=8)
                   {

                       if(!padIdx)
                       {
                           tmp|=(unsigned long long )(0x06)<<(noBytesInWord<<3); //noBytes no noBits
                       }
                       if(padIdx>=noPadding-8){
                            //pad the last byte
                           tmp |= (unsigned long long)(0x01)<<0x3f;
                       }
                       //otherwise leave as zeros
                       padIdx+=8-noBytesInWord;
                   }
               }
               A[x][y] ^=tmp;
               blockCursor +=8;

            }
        }
        //advance message cursor
        cursor += noBytesInBlock;
        //KECCAK-f
        sha3_keccak_f(A);
    }
    //SQUEEZING PHASE
    cursor=0;   //cursor for the output string
    while(cursor<ret_len)
    {
        int blockCursor=0;  //cursor in the block
        int x,y;
        for(y=0;y<5;y++)
        {
            for(x=0;x<5;x++)
            {
                if(blockCursor<r && cursor + blockCursor < ret_len)
                {
                    //copy from state to the output
                    memcpy(*out+cursor+blockCursor,A[x]+y,sizeof(unsigned long long));
                    blockCursor +=8;
                }
                else
                {
                    break;
                }
            }
            if(blockCursor>=r || cursor+blockCursor>=ret_len)
            {
                //either completed block o the output
                break;
            }
        }

        cursor+=r;
        //keccak-f
        sha3_keccak_f(A);
    }
    return ret_len;
}
char hex[16]="0123456789ABCDEF";
void PrintCharArr(unsigned char *arr,int len)
{
	//printf("{ ");
	int i;
	for(i=0;i<len;i++)
	{
		printf("%c%c", hex[arr[i] >> 4] , hex[arr[i] & 0x0f] );
	}
	//printf("%s }\n");
}
int main()
{
	int l=SHA3_L;
	int nr=SHA3_NR;
	int b=SHA3_B;

	//printf("%d %d %d \n",l,nr,b);

	unsigned char *msg="abcdef";    //write the text here
	unsigned char *hash=NULL;

	int len=sha3_hash(msg,strlen(msg),SHA3_256,&hash);
	PrintCharArr(hash,len);

	return 0;
}
