#include <stdio.h>
#include <stdlib.h>
typedef long long BLOCK;

void init(void *myMem)
{
	void *lastBlock = (BLOCK*)myMem + 124999; //pointer to the last 8 bytes
	void *tracker;
	*(int*)lastBlock = 0; //last block point to the first block
	int i;
	for (i = 0; i < 124998; i++)
	{
		tracker = (BLOCK*)myMem + i;
		*(int*)tracker = i + 1; //at init, each block points to the next, the last to first and one before last to none(-1)
	}
	tracker = (BLOCK*)myMem + 124998;
	*(int*)tracker = -1;
}

void* myMalloc(void *myMem)
{
	void *lastBlock = (BLOCK*)myMem + 124999; //pointer to the last 8 bytes
	int idx = *(int*)lastBlock; //read content of the last block
	if (idx == -1)
		return NULL; //memory full
	void *blockToRet = (BLOCK*)myMem + idx; //pointer to return
	int nextIdx = *(int*)blockToRet; //this block contains the next-free block index
	*(int*)lastBlock = nextIdx; //now the last block points to the next free block
	return blockToRet;
}

void myFree(void* myMem, void *toFree)
{
	void *lastBlock = (BLOCK*)myMem + 124999; //pointer to the last 8 bytes
	*(int*)toFree = *(int*)lastBlock; //now the free block points to previous next-free block
	*(int*)lastBlock = ((BLOCK*)toFree - (BLOCK*)myMem); //last block points to the new free block
}

int main()
{
	//init section
	void *myMem = malloc(1000000); //we have 1 MB
	if (!myMem)
		return -1;
	init(myMem);
	//end init section
	
	
	//example of use. You can follow the pointers by debugging
	void *a = myMalloc(myMem);
	void *b = myMalloc(myMem);
	void *c = myMalloc(myMem);
	myFree(myMem, b);
	void *d = myMalloc(myMem);
	//-----------------------
	
	//cleanup section
	free(myMem);
	//end cleanup section
	return 0;
}