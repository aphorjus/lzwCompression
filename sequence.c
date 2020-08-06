#include <stdlib.h>
#include <sequence.h>
#include <string.h>
#include <stdio.h>

/*
 COURSE:        CS 360 Spring 2018
 ASSIGNMENT:    1
 AUTHOR:        Austin Horjus
 */

Sequence* newSequence(unsigned char firstByte) {
	Sequence* newSequence = (Sequence*) malloc(sizeof(Sequence));
	newSequence->count = 1;
	newSequence->data = malloc(newSequence->count);
	newSequence->data[0] = firstByte;

    return newSequence;
}

void deleteSequence(Sequence* sequence) {
	free(sequence->data);
	free(sequence);
}

Sequence* copySequenceAppend(Sequence* sequence, unsigned char addByte) {

	int count = sequence->count;

	Sequence* appSequence = newSequence(0);
	appSequence->count = count+1;

	free(appSequence->data);
	appSequence->data = malloc(count+1);

	for(int i = 0; i < count; i++){
		appSequence->data[i] = sequence->data[i];
	}

	appSequence->data[count] = addByte;

    return appSequence;
}

void outputSequence(Sequence* sequence,
                    int (*writeFunc)(unsigned char c, void* context), void* context) {

	for(int i = 0; i < sequence->count; i++){

		writeFunc(sequence->data[i], context);
	}

}

bool identicalSequences(Sequence* a, Sequence* b) {

	if(b->count != a->count){
		return false;
	}
	for(int i = 0; i < a->count; i++){
		if(a->data[i] != b->data[i]){
			return false;
		}
	}
    return true;
}
