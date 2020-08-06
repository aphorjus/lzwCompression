#include <sequence.h>
#include <bitStream.h>
#include <dict.h>
#include <lzw.h>

#include <stdio.h>

/*
 COURSE:        CS 360 Spring 2018
 ASSIGNMENT:    1
 AUTHOR:        Austin Horjus
 */

Sequence* replaceAppendSequence(Sequence* s, unsigned char c){
	Sequence* temp = copySequenceAppend(s, c);
	deleteSequence(s);

	return temp;
}

Dict* initializeDict(unsigned int size){

	Dict* dict = newDict(size);

	for(unsigned int i = 0; i < 256; i++){
		Sequence* sequence = newSequence((unsigned char) i);
		insertDict(dict, sequence, i);
	}
	return dict;
}

unsigned int twoToThePowerOf(unsigned int x){
	unsigned int retVal = 2;
	for(int i = 1; i < x; i++){
		retVal *= 2;
	}
	return retVal;
}

bool lzwEncode(unsigned int bits, unsigned int maxBits,
               int (*readFunc )(void* context), void* readContext,
               int (*writeFunc)(unsigned char c, void* context), void* writeContext) {

	unsigned int c;
	unsigned int code;
	BitStream* outputBS = openOutputBitStream(writeFunc, writeContext);
	unsigned int tableSize = 1 << maxBits;
	Dict* dict = initializeDict(tableSize);
	int nextCode = 256;

	c = readFunc(readContext);
	Sequence* w = newSequence(c);

	while(c != -1){
		c = readFunc(readContext);
		Sequence* x = copySequenceAppend(w, c);

		if(searchDict(dict, x, &code)){
			deleteSequence(w);
			w = x;
		}
		else{
			searchDict(dict, w, &code);
			outputBits(outputBS, bits, code);

			if(nextCode >= (1 << (bits)) && bits < maxBits){
					printf("nextCode: %d,  ", nextCode);
					printf("bits: %d\n", bits);
					bits++;
			}

			if(nextCode < tableSize){
				insertDict(dict, x, nextCode++);

			}
			else{
				deleteSequence(x);
			}
			deleteSequence(w);
			w = newSequence(c);
		}		
	}
	searchDict(dict, w, &code);
	outputBits(outputBS, bits, code);

	deleteSequence(w);
	deleteDictDeep(dict);
	closeAndDeleteBitStream(outputBS);

	// printDict(dict);

    return true;
}