#include <stdio.h>
#include "stack.h"
#include <limits.h>
#include <stdlib.h>


int getIntNumber()
{
    int n;
    while (1) {
        if (scanf_s("%d", &n) != 0) {
            if ((getchar()) != '\n' || n > INT_MAX || n < INT_MIN) {
                printf("Error: enter the correct number!\n");
                while ((getchar()) != '\n')
                    continue;
            }
            else {
                break;
            }
        }
        else {
            printf("Error: enter the correct number!\n");
            while ((getchar()) != '\n')
                continue;
        }
    }
    return n;
}

Stack* push(struct Stack *p, int value) {
	Stack* t = malloc(sizeof(Stack));
    if (!t) {
        printf("Memory allocation error\n");
        return p;  
    }
	t->info = value;
	t->next = p;
	return t;
}

int pop(Stack** begin) {
    if (*begin == NULL) {
        return -1;
    }
    Stack* t = *begin;
    int data = t->info;
    *begin = (*begin)->next;
    free(t);
    return data;
}

void fillStackFromKeyboard(struct Stack** p) {
    printf("Enter number of nodes: ");
    int n = getIntNumber();
    for (int i = 0; i < n; i++) {
        printf("Info: ");
        *p = push(*p, getIntNumber());
    }
}

void fillAscendingStack(Stack** begin) {
    int input;
    int previous = INT_MIN; 
    int count = 0;
    printf("Enter number of nodes: ");
    int size = getIntNumber();
    while (count < size) {
        printf("Info: ");
        input = getIntNumber();
        if (input > previous) {
            *begin = push(*begin, input);
            previous = input;
            count++;
        }
        else {
            printf("Error!\n");
        }
    }
}

int isEmpty(struct Stack* begin) {
    return begin == NULL;
}

void printStack(struct Stack* begin) {
    if (isEmpty(begin)) {
        puts("Stack is empty!");
        return;
    }

    Stack* t = begin;
    while (t != NULL) {
        printf("%d ", t->info);
        t = t->next;
    }
}

void clearStack(struct Stack** begin) {
    while (*begin != NULL) {  
        Stack* t = *begin;   
        *begin = (*begin)->next; 
        free(t);            
    }
}


int findMostFrequentElement(struct Stack* begin) {
    if (isEmpty(begin)) return -1;
    Stack* current = begin;
    int mostFrequent = current->info;
    int maxCount = 0;
    while (current != NULL) {
        Stack* t = begin;
        int count = 0;
        while (t != NULL) {
            if (t->info == current->info) {
                count++;
            }
            t = t->next;
        }
        if (count > maxCount) {
            maxCount = count;
            mostFrequent = current->info;
        }
        current = current->next; 
    }
    return mostFrequent;
}


Stack* mergeAscendingStacks(struct Stack* stack1, struct Stack* stack2) {
    Stack* result = NULL;

    while (!isEmpty(stack1) || !isEmpty(stack2)) {
        if (stack1 == NULL) {
            result = push(result, pop(&stack2));
        } else if (stack2 == NULL) {
            result = push(result, pop(&stack1));
        } else if (stack1->info >= stack2->info) { 
            result = push(result, pop(&stack1));
        } else {
            result = push(result, pop(&stack2));
        }
    }
    return result;
}


