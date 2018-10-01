#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "wc.h"
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MULTIPLIER (37)

int hashTableSize;

struct node {
	
	// storing the hash key
	unsigned long hashKey;
	
	// storing the number of times word occurs in the array
	int wordCount;

	// storing the actual word
	char *word;

	// pointer to the next node
	struct node *next;
};

struct wc {
	/* you can define this struct to have whatever fields you want. */
	// wc is a pointer to an array of pointers 
	//char* temp;
	
	struct node** arrayOfPointersToNode;
};

unsigned long hash_key_function(char *str);
int hash_code_function(unsigned long hashKey, int size);

// storing elements into the hash table
struct wc *
wc_init(char *word_array, long size)
{
	
	struct wc *wc;

	wc = (struct wc *)malloc(sizeof(struct wc));

	assert(wc);

	// let the size of the hash table array be the total number of words in the array
	
	//wc->temp = malloc();

	hashTableSize = 0;
	
	int maxCharInWord = 0;
	int wordCounter = 0;
	// find number of words there are in the array, by counting the number of space
	for (int i = 0; i<size; i++){
		if (isspace(word_array[i])){
			hashTableSize++;
			if (wordCounter>=maxCharInWord){
				maxCharInWord = wordCounter;
			}
			wordCounter = 0;
		}
		else{
			wordCounter++;
		}
	}
	
	// allocating memory for array of pointers in wc
	
	wc->arrayOfPointersToNode= (struct node **)malloc(sizeof(struct node*) * hashTableSize);
	
	// insert into the hashtable
	
	// if not out of memory
	if (wc->arrayOfPointersToNode!=NULL){

	// need a string (char array) to hold each word in the array
	char *eachWord = (char*)malloc((sizeof(char) * maxCharInWord) + 1);

	int charCountInWord = 0;

	//if not out of memory for string
	if (eachWord !=NULL){
	for (int i = 0; i<size; i++){

		if (isspace(word_array[i])){
									
			if (charCountInWord<maxCharInWord){
			
			eachWord[charCountInWord] = '\0';

			}
			else{
			eachWord[charCountInWord+1] = '\0';
			}

			// if eachWord only has a space, don't account for this		
			if (charCountInWord!=0){

			// insert into hash table, need to insert the hash key of the word, and then increment the count
			// dereference the char array and get the hash key
			int hashKey = hash_key_function(eachWord);
			
			int hashArrayIndex = hash_code_function(hashKey, hashTableSize);

			// do the absolute of the hash table 
			hashArrayIndex = abs(hashArrayIndex);
		
			// update the pointer in the array index

			struct node* current = wc->arrayOfPointersToNode[hashArrayIndex];			
			struct node* nodeToBeAdded = (struct node*)malloc(sizeof(struct node));	
			
			// if not out of memory
			if (nodeToBeAdded!=NULL){		

			// first node at this index
			if (current == NULL){

				wc->arrayOfPointersToNode[hashArrayIndex] = nodeToBeAdded;
				wc->arrayOfPointersToNode[hashArrayIndex]->next = NULL;
				wc->arrayOfPointersToNode[hashArrayIndex]->hashKey = hashKey;
				wc->arrayOfPointersToNode[hashArrayIndex]->word = strdup(eachWord);
				wc->arrayOfPointersToNode[hashArrayIndex]->wordCount = 1;
			}
							
			else {

			// iterate to find the correct node with the corresponding hashkey
			while (current->next!=NULL&&strcmp(eachWord, current->word)!=0){
				current = current->next;
			}
			
			// string comparison is the same, increment word count
			if (strcmp(eachWord, current->word)==0){
			// adding the node to the pointer of node		
			current->wordCount++;	

			}

			// need to insert another node to the linked list
			else if (current->next==NULL){
				current->next = nodeToBeAdded;
				current->next->hashKey = hashKey;
				current->next->wordCount = 1;
				current->next->next = NULL;	

				current->next->word = strdup(eachWord);

				}

			}
			// set the memory of the string to be NULL
			charCountInWord = 0;
			}
					
		 }	
			
		}
		else {
	
			eachWord[charCountInWord] = word_array[i];
			charCountInWord++;
		}

	  }

	}

  }

	return wc;
}

// outputing all the count of every word in the hash table
void
wc_output(struct wc *wc)
{
	// need to interate through the hash table


	for (int i = 0; i<hashTableSize; i++){

		if (wc->arrayOfPointersToNode[i]!=NULL){

			struct node* current = wc->arrayOfPointersToNode[i];
			while (current!=NULL){

				// loop through the string to print the word

				printf("%s:%d\n", current->word, current->wordCount);
				current = current->next;
			}
		}
		
	}
}

void
wc_destroy(struct wc *wc)
{
	
	// Iterate through the hash table and for each array of pointer, free all the linked lists	
	for (int i = 0; i<hashTableSize; i++){

		if (wc->arrayOfPointersToNode[i]!=NULL){

			struct node* current = wc->arrayOfPointersToNode[i];
			struct node* nextNode;
			while (current!=NULL){
				nextNode = current->next;				
				free(current);
				current = nextNode;		
			}
			
		}
		
	}

	// freeing *wc
	free(wc);
}


// converting from a given word to a hash key of unsigned long
unsigned long hash_key_function(char *str){
	unsigned long hash = 5381;
    int c = *str;

    while (c!=0){
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
		c = *str++;
  	}
	return hash;
}

// converting from key to hash code which is the index of the hash table's array
int hash_code_function(unsigned long hashKey, int size){
	int hashCode;
	hashCode = (int)hashKey%size;
	return hashCode;	
}
