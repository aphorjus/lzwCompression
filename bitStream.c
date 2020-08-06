#include <stdlib.h>
#include <bitStream.h>
#include <stdio.h>


/*
 COURSE:        CS 360 Spring 2018
 ASSIGNMENT:    1
 AUTHOR:        Austin Horjus
*/


BitStream* openInputBitStream(int (*readFunc)(void* context), void* context) {

	BitStream* newBitStream = (BitStream*) malloc(sizeof(BitStream));

	newBitStream->context = context;
	newBitStream->readFunc = readFunc;
	newBitStream->writeFunc = NULL;
	newBitStream->buffer = 0;
	newBitStream->bitsInBuffer = 0;
	newBitStream->isOutputStream = false;

	return newBitStream;
}

BitStream* openOutputBitStream(int (*writeFunc)
	(unsigned char c,void* context),void* context) {

	BitStream* newBitStream = (BitStream*) malloc(sizeof(BitStream));

	newBitStream->context = context;
	newBitStream->readFunc = NULL;
	newBitStream->writeFunc = writeFunc;
	newBitStream->buffer = 0;
	newBitStream->bitsInBuffer = 0;
	newBitStream->isOutputStream = true;

	return newBitStream;
}

void closeAndDeleteBitStream(BitStream* bs) {
	free(bs);
}

void outputBits(BitStream* bs, unsigned int nBits, unsigned int code) {

	if(!bs->isOutputStream){
		fprintf(stderr, "Wrong BitStream type: attempting output from read stream\n");
		return;
	}

	unsigned char MSB;
	int n = bs->bitsInBuffer + nBits;
	unsigned int buffer = (bs->buffer << nBits)|code;

	while(n > 8){
		MSB = (buffer >> (n-8));
		bs->writeFunc(MSB, bs->context);
		buffer ^= (MSB << (n-8));
		n -= 8;
	}
	bs->bitsInBuffer = n;
	bs->buffer = buffer;

	return;
}

bool readInBits(BitStream* bs, unsigned int nBits, unsigned int* code) {


	if(bs->isOutputStream){
		fprintf(stderr, "Wrong BitStream type: attempting read from output stream\n");
		return false;
	}
	if(!bs->context){
		fprintf(stderr, "ERROR: NULL context in input bs\n");
		return false;
	}


	unsigned int result;
	int newByte;

	while(nBits > bs->bitsInBuffer){

		newByte = bs->readFunc(bs->context);

		if(newByte == -1){
			return false;
		}

		bs->buffer = (bs->buffer << 8)|newByte;
		bs->bitsInBuffer += 8;
	}
	bs->bitsInBuffer -= nBits;
	result = bs->buffer >> bs->bitsInBuffer;
	*code = result;

	bs->buffer = ((result << bs->bitsInBuffer) ^ bs->buffer);

	// printf("buffer:%x, bits In it:%d\n", bs->buffer, bs->bitsInBuffer);

	// int MSB;
	// int LSB;

	// // bs->buffer

	// MSB = bs->readFunc(bs->context);
	// LSB = bs->readFunc(bs->context);

	// if (MSB == -1 || LSB == -1) {
	// 	return false;
	// }
	// result = (MSB << 8)|LSB;
	// *code = result;

	return true;
}

//c = buffer >> n-8
//ouput(c)
//buffer ^= (c << n-8)
