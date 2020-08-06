#include <sequence.h>
#include <bitStream.h>
#include <lzw.h>
#include <stdlib.h>

#include <stdio.h>

/*
 COURSE:        CS 360 Spring 2018
 ASSIGNMENT:    1
 AUTHOR:        Austin Horjus
 */

Sequence** initializeTable(unsigned int size, unsigned int* codesInserted){

	Sequence** table = (Sequence**) calloc(size, sizeof(Sequence*));

	for(unsigned int i = 0; i < 256; i++){
		Sequence* sequence = newSequence((unsigned char) i);
		table[i] = sequence;
		*codesInserted += 1;
	}
	return table;
}

void freeTable(Sequence** table, unsigned int size){
	for(int i = 0; i < size; i++){
		if(table[i]){
			deleteSequence(table[i]);
		}
	}
	free(table);
}

unsigned int twoToThePowerOff(unsigned int x){
	unsigned int retVal = 2;
	for(int i = 1; i < x; i++){
		retVal *= 2;
	}
	return retVal;
}

bool lzwDecode(unsigned int bits, unsigned int maxBits,
               int (*readFunc )(void* context), void* readContext,
               int (*writeFunc)(unsigned char c, void* context), void* writeContext) {

	BitStream* inputBS = openInputBitStream(readFunc, readContext);
	unsigned int tableSize = 1 << maxBits;
	unsigned int codesInserted = 0;

	Sequence** table = initializeTable(tableSize, &codesInserted);
	unsigned int previousCode;
	unsigned int currentCode;
	unsigned char c;

	bool isNull = false;

	printf("%s\n", "about to read in some bits");
	if(codesInserted >= (1 << bits)){
		printf("codesInserted:%u, bits:%d", codesInserted, bits);
		bits++;
	}
	readInBits(inputBS, bits, &previousCode);
	outputSequence(table[previousCode], writeFunc, writeContext);

	while(readInBits(inputBS, bits, &currentCode)){
		// printf("%p: %u\n", table[currentCode], currentCode);

		if(table[currentCode] != NULL){
			// printf("%s\n", "hello");
			isNull = false;
			c = table[currentCode]->data[0];
		}
		else{
			printf("%s\n", "NULL NULL NULL NULL NULL");
			isNull = true;
			c = table[previousCode]->data[0];
		}
		if(codesInserted >= (1 << (bits)) && bits < maxBits){
				printf("codesInserted:%u, bits:%d\n", codesInserted, bits);
				bits++;
			}

		if(isNull)
			printf("%d: %p\n", currentCode, table[currentCode]);

		if(codesInserted < tableSize){
			Sequence* w = copySequenceAppend(table[previousCode], c);
			table[codesInserted++] = w;
		}
		// printf("%d\n", bits);
		// printf("%s", "data:  ");
		// for(int i = 0; i < table[currentCode]->count; i++){
		// 	printf("%c", table[currentCode]->data[i]);	
		// }
		// printf("\n");
		if(isNull)
			printf("codesInserted: %d\ncurrentCode  : %d: %p\n", codesInserted, currentCode, table[currentCode]);

		// if(currentCode != codesInserted){
		// 	outputSequence(table[codesInserted], writeFunc, writeContext);
		// }
		// else
			outputSequence(table[currentCode], writeFunc, writeContext);
		
		previousCode = currentCode;
	}
	closeAndDeleteBitStream(inputBS);
	freeTable(table, tableSize);
    return true;
}
