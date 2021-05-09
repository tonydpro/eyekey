#include "chiffrage.h"

void crypter(unsigned char* str,char* key,int t)
{
	int c;
	for (c = 0 ; c < t-1 ; c++)
		melange(str+c,str+c+1,key);
}

void decrypter(unsigned char* str,char* key,int t)
{
	int c;
	for (c = t-1 ; c > 0 ; c--)
		melange(str+c-1,str+c,key);
}

void modifier_bit(unsigned char* c,int bit,int valeur)
{
    unsigned char masque = (valeur == 0) << (7-bit);
    *c ^= masque;
}

void melange(unsigned char* c1,unsigned char* c2,char* key)
{
	int c;
	for (c = 0 ; c < 7 ; c++)
	{
        unsigned char sto_valeur = (*c1>>(c-7))&1;
        modifier_bit(c1,c,(int)(key[c]-'0'-1));
        modifier_bit(c2,(int)(key[c]-'0'-1),sto_valeur);
	}
}

void convCharOctet(int dec,unsigned char* bin)
{
    int Q = 0,T;
    for (T = 7 ; T != -1 ; T --)
    {
        Q = dec / 2;
        bin[T] = '0' + dec - 2*Q;
        dec = Q;
    }
}

int byteToNum(unsigned char* str)
{
    int i;
    int num = 0,c;

	for (i = 7 ; i > -1 ; i--)
    {
        c = str[i];
        if (c != '0' && c != '1')
            return 0;
        num += (c - '0')*pow(2,7-i);
    }
	convCharOctet(num,str);

    return num;
}
