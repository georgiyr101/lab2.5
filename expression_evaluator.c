#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "expression_evaluator.h"

double popDouble(Stack** begin) {
	if (*begin == NULL) {
		return -1;
	}
	Stack* t = *begin;
	double data = t->info;
	*begin = (*begin)->next;
	free(t);
	return data;
}

int isStackEmpty(struct Stack* begin) {
	return begin == NULL;
}

OperatorStack* pushOperator(OperatorStack* p, char value) {
	OperatorStack* t = malloc(sizeof(OperatorStack));
	if (!t) return p;
	t->info = value;
	t->next = p;
	return t;
}

char popOperator(OperatorStack** p) {
	if (isOperatorStackEmpty(*p)) return '\0';
	OperatorStack* t = *p;
	char data = t->info;
	*p = (*p)->next;
	free(t);
	return data;
}

int isOperatorStackEmpty(OperatorStack* p) {
	return p == NULL;
}

void clearOperatorStack(OperatorStack** p) {
	while (*p) {
		OperatorStack* t = *p;
		*p = (*p)->next;
		free(t);
	}
}

int my_strlen(const char* str) {
	int length = 0;
	while (str[length] != '\0') {
		length++;
	}
	return length;
}

int getPriority(char operator) {
	if (operator == '+' || operator == '-') return 1;
	if (operator == '*' || operator == '/') return 2;
	return 0;
}

char* infixToPostfix(char* infix, int* errorPosition) {
	int length = my_strlen(infix);
	char* postfix = (char*)malloc(sizeof(char) * (2 * length + 1));
	if (!postfix) {
		*errorPosition = -1;
		return NULL;
	}

	int j = 0;
	OperatorStack* opStack = NULL;

	for (int i = 0; i < length; i++) {
		if (isspace(infix[i])) continue;

		if (isdigit(infix[i]) || infix[i] == '.') {
			postfix[j++] = infix[i];
			while (isdigit(infix[i + 1]) || infix[i + 1] == '.') {
				postfix[j++] = infix[++i];
			}
			postfix[j++] = ' ';
		}
		else if (infix[i] == '(') {
			opStack = pushOperator(opStack, infix[i]);
		}
		else if (infix[i] == ')') {
			while (!isOperatorStackEmpty(opStack) && opStack->info != '(') {
				postfix[j++] = popOperator(&opStack);
				postfix[j++] = ' ';
			}
			if (isOperatorStackEmpty(opStack)) {
				*errorPosition = i;
				free(postfix);
				clearOperatorStack(&opStack);
				return NULL;
			}
			popOperator(&opStack);
		}
		else if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/') {
			if (i == 0 || infix[i - 1] == '(' || infix[i - 1] == '+' || infix[i - 1] == '-' || infix[i - 1] == '*' || infix[i - 1] == '/') {
				postfix[j++] = infix[i];
				i++;
				while (isdigit(infix[i]) || infix[i] == '.') {
					postfix[j++] = infix[i++];
				}
				postfix[j++] = ' ';
				i--;
			}
			else {
				while (!isOperatorStackEmpty(opStack) && getPriority(opStack->info) >= getPriority(infix[i])) {
					postfix[j++] = popOperator(&opStack);
					postfix[j++] = ' ';
				}
				opStack = pushOperator(opStack, infix[i]);
			}
		}
		else {
			*errorPosition = i;
			free(postfix);
			clearOperatorStack(&opStack);
			return NULL;
		}
	}

	while (!isOperatorStackEmpty(opStack)) {
		if (opStack->info == '(') {
			*errorPosition = 0;
			free(postfix);
			clearOperatorStack(&opStack);
			return NULL;
		}
		postfix[j++] = popOperator(&opStack);
		postfix[j++] = ' ';
	}

	postfix[j] = '\0';
	return postfix;
}

EvaluationResult evaluatePostfix(char* postfix) {
	Stack* numStack = NULL;
	int length = my_strlen(postfix);
	EvaluationResult evalResult = { 0.0, -1 }; 
	int i = 0; 
	int operatorFound = 0; 

	while (i < length) {
		if (isdigit(postfix[i]) || postfix[i] == '.' || (postfix[i] == '-' && isdigit(postfix[i + 1]))) {
			char numStr[32];
			int k = 0;
			if (postfix[i] == '-' && isdigit(postfix[i + 1])) {
				numStr[k++] = postfix[i++];
			}
			while (isdigit(postfix[i]) || postfix[i] == '.') {
				numStr[k++] = postfix[i++];
			}
			numStr[k] = '\0';
			numStack = push(numStack, atof(numStr));
		}
		else if (postfix[i] == '+' || postfix[i] == '-' || postfix[i] == '*' || postfix[i] == '/') {
			operatorFound = 1; 
			if (isStackEmpty(numStack)) {
				evalResult.errorPosition = i;
				clearStack(&numStack);
				return evalResult;
			}
			double b = popDouble(&numStack);
			if (isStackEmpty(numStack)) {
				evalResult.errorPosition = i;
				clearStack(&numStack);
				return evalResult;
			}
			double a = popDouble(&numStack);
			if (postfix[i] == '/') {
				if (b == 0) {
					evalResult.errorPosition = i;
					clearStack(&numStack);
					return evalResult;
				}
			}
			double result;
			if (postfix[i] == '+') result = a + b;
			else if (postfix[i] == '-') result = a - b;
			else if (postfix[i] == '*') result = a * b;
			else result = a / b;
			numStack = push(numStack, result);
		}
		i++;
	}

	if (!operatorFound) {
		evalResult.errorPosition = 0; 
		clearStack(&numStack);
		return evalResult;
	}
	if (isStackEmpty(numStack)) {
		evalResult.errorPosition = length;
		return evalResult;
	}
	evalResult.result = popDouble(&numStack);
	clearStack(&numStack);
	return evalResult;
}

void writeExpressionsToFile() {
	FILE* inputFile = fopen("input.txt", "w");

	if (!inputFile) {
		printf("Ошибка открытия файла input.txt\n");
		return;
	}

	char expression[256];
	printf("Введите выражения (для завершения введите 'exit'):\n");

	while (1) {
		fgets(expression, sizeof(expression), stdin); // Читаем строку из стандартного ввода

		// Удаляем символ новой строки, если он есть
		size_t len = strlen(expression);
		if (len > 0 && expression[len - 1] == '\n') {
			expression[len - 1] = '\0';
		}

		if (strcmp(expression, "exit") == 0) { // Проверяем, введено ли "exit"
			break; // Завершаем цикл, если введено "exit"
		}

		fprintf(inputFile, "%s\n", expression); // Записываем выражение в файл
	}

	fclose(inputFile); // Закрываем файл
	printf("Выражения записаны в файл input.txt\n");
}

void printFile(const char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	puts("Содержимое файла:");
	char ch;
	while ((ch = fgetc(file)) != EOF) {
		putchar(ch);
	}

	fclose(file);
}