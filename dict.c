#include <stdlib.h>
#include <stdio.h>
#include <dict.h>
#include <sequence.h>

/*
 COURSE:        CS 360 Spring 2018
 ASSIGNMENT:    1
 AUTHOR:        Austin Horjus
 */

// Bernstein, for hash function

///////////////////////////////////////////////////////////////////////////
//////							Linked List							 //////
///////////////////////////////////////////////////////////////////////////

Entry* newEntry(Sequence* sequence, int code){

	Entry* newEntry = (Entry*) malloc(sizeof(Entry));
	newEntry->key = sequence;
	newEntry->value = code;
	newEntry->next = NULL;

	return newEntry;
}

bool equalEntry(Entry* eOne, Entry* eTwo){
	if (eOne->value == eTwo->value && 
					identicalSequences(eOne->key, eTwo->key)){
		return true;
	}
	return false;
}

bool keyInList(Sequence* key, Entry* list, unsigned int* code){

	while(list != NULL){
		if( identicalSequences(key, list->key) ){
			*code = list->value;
			return true;
		}
		list = list->next;
	}
	return false;
}

void insertList(Entry* newEntry, Entry* entry){
	newEntry->next = entry;
}

void deleteEntryList(Entry* entry){

	if(entry->next != NULL){
		deleteEntryList(entry->next);
	}
	deleteSequence(entry->key);
	free(entry);
}

void printList(Entry* list){

	printf(" | %x,  -", list->value);

	for(int i = 0; i < list->key->count; i++){
		printf("%c", list->key->data[i]);
	}
	if(list->next){
		printList(list->next);
		return;
	}
	printf("-\n");
}

///////////////////////////////////////////////////////////////////////////
//////							Hash Table							 //////
///////////////////////////////////////////////////////////////////////////

Dict* newDict(unsigned int hashSize) {

	Dict* newDict = (Dict*) malloc(sizeof(Dict));
	newDict->table = (Entry**) calloc(hashSize, sizeof(Entry*));
	newDict->size = hashSize;

    return newDict;
}

unsigned long hash(unsigned char *str, int count) {

    unsigned long hash = 5381;

    for(int i = 0; i < count; i++){
		hash = ((hash << 5) + hash) + str[i]; /* hash * 33 + c */
    }
	return hash;
}

int getHashedIndex(unsigned char* key, int count, int tableSize){

	unsigned long hashCode = hash(key, count);
	return hashCode % tableSize;
}

void printDict(Dict* dict){

	for(int i = 0; i < dict->size; i++){
		if(dict->table[i] != NULL){
			printf("%d ::", i);
			printList(dict->table[i]);
		}
	} 
}

void deleteDictDeep(Dict* dict) {

	// printDict(dict);

	for(int i = 0; i < dict->size; i++){
		if(dict->table[i] != NULL){
			deleteEntryList(dict->table[i]);
		}
	}
	free(dict->table);
	free(dict);
}

bool searchDict(Dict* dict, Sequence* key, unsigned int* code) {

	int index = getHashedIndex(key->data, key->count, dict->size);

	if(dict->table[index] != NULL && keyInList(key, dict->table[index], code)){
		return true;
	}
	return false;
}

void insertDict(Dict* dict, Sequence* key, unsigned int code) {

	int index = getHashedIndex(key->data, key->count, dict->size);
	Entry* newE = newEntry(key, code);

	if(dict->table[index] != NULL){
		insertList(newE, dict->table[index]);
	}
	dict->table[index] = newE;

	// printf("%d ::", index);
	// printList(dict->table[index]);
}
