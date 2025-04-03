#ifndef STACK_H
#define STACK_H

typedef struct Stack {
	int info;
	struct Stack* next;
} Stack;


int getIntNumber();

Stack* push(struct Stack* p, int value);

void fillStackFromKeyboard(struct Stack** p);

void fillAscendingStack(Stack** begin);

int isEmpty(struct Stack* begin);

void printStack(struct Stack* begin);

void clearStack(struct Stack** begin);

int findMostFrequentElement(struct Stack* begin);

Stack* mergeAscendingStacks(struct Stack* stack1, struct Stack* stack2);

#endif
