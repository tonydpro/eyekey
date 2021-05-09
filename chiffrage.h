#ifndef CHIFFRAGE_H_INCLUDED
#define CHIFFRAGE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

void crypter(unsigned char*,char*n,int);
void decrypter(unsigned char*,char*,int);
void modifier_bit(unsigned char*,int,int);
int entAleat(int,int);
void aleat(char*);
void melange(unsigned char*,unsigned char*,char*);
void convCharOctet(int,unsigned char*);
int byteToNum(unsigned char*);
void cryptercle(char*);

#endif // CHIFFRAGE_H_INCLUDED
