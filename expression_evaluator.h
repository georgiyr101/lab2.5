#ifndef EXPRESSION_EVALUATOR_H
#define EXPRESSION_EVALUATOR_H
#include "stack.h"


typedef struct OperatorStack {
	char info;
	struct OperatorStack* next;
} OperatorStack;

typedef struct {
	double result;
	int errorPosition;
} EvaluationResult;

double popDouble(Stack** begin);

int isStackEmpty(struct Stack* begin);

OperatorStack* pushOperator(OperatorStack* p, char value);

char popOperator(OperatorStack** p);

isOperatorStackEmpty(OperatorStack* p);

int my_strlen(const char* str);

int getPriority(char operator);

void clearOperatorStack(OperatorStack** p);

char* infixToPostfix(char* infix, int* errorPosition);

void writeExpressionsToFile();

void printFile(const char* filename);

EvaluationResult evaluatePostfix(char* postfix);


#endif
